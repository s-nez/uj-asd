#!/usr/bin/env perl 
use strict;
use warnings;
use feature 'say';
use Data::Dumper;

use constant prompt   => '> ';
use constant min_keys => 3;
use constant max_keys => 6;
my $tree = {
    keys => [],
    refs => []
};

sub is_empty {
    my ($node) = @_;

    # If the root doesn't have any keys, the tree is empty
    return scalar @{ $node->{keys} } == 0;
}

sub is_leaf {
    my ($node) = @_;

    # The node is a leaf if it doesn't have any refs
    return @{ $node->{refs} } == 0;
}

# Find a place to insert $elem into $array (reference),
# may be the end of array
sub ordered_find {
    my ($elem, $array) = @_;
    my $index = 0;
    while ($index < @$array and $array->[$index] < $elem) {
        ++$index;
    }
    return $index;
}

sub key_insert {
    my ($elem, $node) = @_;
    my $keys = $node->{keys};

    my $key_index = ordered_find($elem, $node->{keys});

    # Do not insert duplicate elements
    unless (defined $keys->[$key_index] and $keys->[$key_index] == $elem) {

        # Insert $elem at $key_index, works at the end of array as well
        splice @$keys, $key_index, 0, $elem;
    }
    return;
}

sub exists_in_tree {
    my ($elem, $tree) = @_;
    return 0 unless defined $tree;

    my $index = ordered_find($elem, $tree->{keys});
    my $found_key = $tree->{keys}->[$index];

    return 1 if (defined $found_key and $found_key == $elem);
    return exists_in_tree($elem, $tree->{refs}->[$index]);
}

sub overflow_fixup {
    my ($node, $child, $child_index) = @_;

    if (@{ $child->{keys} } > max_keys) {

        # Remove the right half of the child and make a new node from it
        my $right = {
            keys => [ splice @{ $child->{keys} }, min_keys + 1, min_keys ],
            refs => []    # Refs empty by default in case child is a leaf
        };

        # Move the right half of a non-leaf childs references to the new node
        unless (is_leaf($child)) {
            $right->{refs} =
              [ splice @{ $child->{refs} }, min_keys + 1, min_keys + 1 ];
        }

        # The right half was removed, so childs last element is the median
        my $median = pop @{ $child->{keys} };

        # Insert the median into the parent
        splice @{ $node->{keys} }, $child_index, 0, $median;

        # Insert a ref to both parts of the split child
        # in place of its previous reference
        splice @{ $node->{refs} }, $child_index, 1, $child, $right;
    }
    return;
}

sub insert {
    my ($elem, $root) = @_;
    internal_insert($elem, $root);
    my $candidate_root = { keys => [], refs => [] };
    overflow_fixup($candidate_root, $root, 0);

    # If there are any new elements in the candidate root,
    # it becomes the new root of the tree
    unless (is_empty($candidate_root)) {
        $_[1] = $candidate_root;    # Direct access to $_ changes the arg value
    }
    return;
}

sub internal_insert {
    my ($elem, $node) = @_;
    if (is_leaf($node)) {
        key_insert($elem, $node);
    } else {
        my $index = ordered_find($elem, $node->{keys});
        my $child = $node->{refs}->[$index];
        internal_insert($elem, $child);
        overflow_fixup($node, $child, $index);
    }

}

sub key_remove {
    my ($elem, $node) = @_;
    my $keys = $node->{keys};

    my $key_index = ordered_find($elem, $node->{keys});
    if (defined $keys->[$key_index] and $keys->[$key_index] == $elem) {
        splice @$keys, $key_index, 1;
    }
    return;
}

sub has_extra_keys {
    my ($node) = @_;
    return (defined $node and @{ $node->{keys} } > min_keys);
}

sub rotate_left {
    my ($target, $parent, $target_index) = @_;
    my $source = $parent->{refs}->[ $target_index + 1 ];

    # Place separator at the end of the left child
    push @{ $target->{keys} }, $parent->{keys}->[$target_index];

    # Remove the first element from the right child
    # and make it the new separator
    $parent->{keys}->[$target_index] = shift @{ $source->{keys} };

    # Take the first child from the source node
    # and append it to the target node
    my $first_child = shift @{ $source->{refs} };
    push @{ $target->{refs} }, $first_child if defined $first_child;
}

sub rotate_right {
    my ($source, $parent, $source_index) = @_;
    my $target = $parent->{refs}->[ $source_index + 1 ];

    # Prepend the separator to the right child
    unshift @{ $target->{keys} }, $parent->{keys}->[$source_index];

    # Remove the last element from the left child
    # and make it the new separator
    $parent->{keys}->[$source_index] = pop @{ $source->{keys} };

    # Take the last child of the source node
    # and prepend it to the target node
    my $last_child = pop @{ $source->{refs} };
    unshift @{ $target->{refs} }, $last_child if defined $last_child;
}

sub merge {
    my ($node, $parent, $node_index) = @_;
    my $sibling   = $parent->{refs}->[ $node_index + 1 ];    # Right sibling
    my $separator = $parent->{keys}->[$node_index];

    # Append the separator and all of siblings keys and refs to $node
    push @{ $node->{keys} }, ($separator, @{ $sibling->{keys} });
    push @{ $node->{refs} }, @{ $sibling->{refs} } if defined $sibling->{refs};

    # Remove the separator and the ref to sibling from parent
    splice @{ $parent->{keys} }, $node_index, 1;
    splice @{ $parent->{refs} }, $node_index + 1, 1;
}

sub underflow_fixup {
    my ($node, $child, $child_index) = @_;

    if (@{ $child->{keys} } < min_keys) {
        my $right_sibling = $node->{refs}->[ $child_index + 1 ];
        if (has_extra_keys($right_sibling)) {
            rotate_left($child, $node, $child_index);
            return;
        }

        my $left_sibling;

        # Avoid taking the last element (index -1) as left sibling
        $left_sibling = $node->{refs}->[ $child_index - 1 ] if $child_index;
        if (has_extra_keys($left_sibling)) {
            rotate_right($left_sibling, $node, $child_index - 1);
            return;
        }

        if (defined $right_sibling) {
            merge($child, $node, $child_index);
        } elsif (defined $left_sibling) {
            merge($left_sibling, $node, $child_index - 1);
        }
    }
    return;
}

# Remove the smallest element from given subtree and return it's value
sub get_new_separator {
    my ($node) = @_;
    if (is_leaf($node)) {
        return shift @{ $node->{keys} };
    } else {
        my $separator = get_new_separator($node->{refs}->[0]);
        underflow_fixup($node, $node->{refs}->[0], 0);
        return $separator;
    }
}

sub remove {
    my ($elem, $root) = @_;
    internal_remove($elem, $root);

    # If the root is empty and has an unempty child, the child becomes the root
    if (is_empty($root)) {
        $_[1] = $root->{refs}->[0] if defined $root->{refs}->[0];
    }
    return;
}

sub internal_remove {
    my ($elem, $node) = @_;
    if (is_leaf($node)) {
        key_remove($elem, $node);
    } else {
        my $keys = $node->{keys};
        my $index = ordered_find($elem, $keys);

        # If the element is found, get it's inorder successor (smallest
        # element from the right subtree) and make it the new separator.
        if (defined $keys->[$index] and $keys->[$index] == $elem) {
            $keys->[$index] = get_new_separator($node->{refs}->[ $index + 1 ]);
            underflow_fixup($node, $node->{refs}->[ $index + 1 ], $index + 1);
        } else {
            my $child = $node->{refs}->[$index];
            internal_remove($elem, $child);
            underflow_fixup($node, $child, $index);
        }
    }
}

sub draw {
    my ($root) = @_;
    my @canvas;
    internal_draw($root, 0, \@canvas);
    say foreach @canvas;
}

sub internal_draw {
    my ($node, $level, $canvas) = @_;
    return unless defined $node;

    push @{$canvas}, '' while (@{$canvas} <= $level);
    if (is_leaf($node)) {
        $canvas->[$level] .= "(@{$node->{keys}}) ";
    } else {
        foreach my $index (keys @{ $node->{keys} }) {
            internal_draw($node->{refs}->[$index], $level + 1, $canvas);
            $canvas->[$level] .= ' ' x
              ((length $canvas->[ $level + 1 ]) - (length $canvas->[$level]));
            $canvas->[$level] .= '(' if ($index == 0);
            $canvas->[$level] .= $node->{keys}->[$index];
            if ($index == $#{ $node->{keys} }) {
                $canvas->[$level] .= ')';
            } else {
                $canvas->[$level] .= ' ';
            }
            $canvas->[ $level + 1 ] .= ' ' x
              ((length $canvas->[$level]) - (length $canvas->[ $level + 1 ]));
        }
        internal_draw($node->{refs}->[ $#{ $node->{refs} } ],
            $level + 1, $canvas);
    }
}

print prompt;
while (<>) {
    chomp;
    if (/\A\d+\z/ and $_ >= 0 and $_ <= 9999) {
        if (exists_in_tree($_, $tree)) {
            remove($_, $tree);
        } else {
            insert($_, $tree);
        }

        #print Dumper $tree;
        draw($tree);
    }
    print prompt;
}
print "\n";
