Replicate Paragraphs Extends Replicate module to manage the cloning of 
paragraphs_item entities and fields.

Basics:
-------
When you clone an entity (node, taxonomy term, ...) containing a 
paragraphs reference, the paragraphs items are not duplicated, 
and the cloned entity still references the same paragraphs than 
the original entity.
This poses great issues as any modification on the paragraphs will impact 
all the duplicated entities.

Replicate Paragraphs correct that behavior and implements a clean 
duplication of paragraphss.
The replication is recursive, i.e will work on paragraphs containing
paragraphss and so on.


Dependencies:
-------------
-Replicate
-Entity API
-Paragraphs

Thanks:
-------
-Vincent Bouchet for his help on the coding and testing of this module
-All the people involved on the discussion: http://drupal.org/node/1233256, 
the first draft of this module was based on the patches submitted here
