---
layout: post
title: The MealMaster file format
category: software
image: /pics/anymeal.png
---

The MealMaster file format is a popular file format for sharing recipes.
MealMaster is the file format introduced by the MealMaster software developed by Episoft Systems.

The website of Episoft Systems, which provided the original MealMaster software for DOS, is not online any more.
Fortunately a [copy of the website][1] is still available on the Internet Archive.

<h4>Grammar</h4>

Here is an [ABNF grammar][2] of the MealMaster file format:

```
Mealmaster   = recipe Mealmaster
               / *( VCHAR / SP ) eol Mealmaster
               /
recipe       = header title categories servings ingredients instructions footer
header       = separator *( VCHAR / SP ) "Meal-Master" *( VCHAR / SP ) eol
title        = '     Title: ' 0*60( VCHAR / SP ) eol
categories   = 'Categories: ' categlist eol
categlist    = 1*11( VCHAR / SP ) ["," categlist]
servings     = '  Servings: ' 1*4( DIGIT ) [ SP [ 1*10( VCHAR / SP ) ] ] eol
ingredients  = onecolumn
               / twocolumn
onecolumn    = section onecolumn
               / ingredone eol onecolumn
               / *( SP ) eol onecolumn
               /
twocolumn    = section twocolumn
               / ingredtwo SP ingredone eol twocolumn
               / ingredone eol [ *( eol ) section twocolumn ]
               /
ingredone    = amount SP unit SP 1*28( VCHAR / SP )
ingredtwo    = amount SP unit SP 28( VCHAR / SP )
amount       = 7( SP / DIGIT / "." / "/" )
unit         = 'x ' / 'sm' / 'md' / 'lg' / 'cn' / 'pk' / 'pn' / 'dr' / 'ds'
               / 'ct' / 'bn' / 'sl' / 'ea' / 't ' / 'ts' / 'T ' / 'tb' / 'fl'
               / 'c ' / 'pt' / 'qt' / 'ga' / 'oz' / 'lb' / 'ml' / 'cb' / 'cl'
               / 'dl' / 'l ' / 'mg' / 'cg' / 'dg' / 'g ' / 'kg' / '  '
instructions = 0*255( VCHAR ) eol instructions
               / section instructions
               /
section      = separator *( VCHAR / SP ) eol
footer       = separator eol
separator    = 'MMMMM'
               / '-----'
eol          = [ CR ] LF
```

The grammar uses several rules from the ABNF standard:

* **/** is used to specify alternative rules.
* **VCHAR** stands for any printable character (except whitespace)
* **DIGIT** is a digit (one of 0,1,2,3,...,9).
* **SP** is the space character.
* **CR** means carriage return (0x0D).
* **LF** is the line-feed character (0x0A).
* **\*( VCHAR / SP )** means any number of printable and space characters or no characters at all.
* **1\*4( DIGIT )** is used to specify one to four digits.
* **[ CR ]** is an optional carriage return character.
* **5( 'M' )** is exactly 5 capital "M"-characters.

<h4>Units</h4>

The following table shows the meaning of each of the unit tokens:

<table border="1" style="text-align: left; background-color: rgb(255, 255, 221);">
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      special tokens
    </th>
  </tr>
  <tr>
    <td><b>x </b></td><td>"per serving" (quantity not to be scaled)</td>
  </tr>
  <tr>
    <td><b>  </b></td><td>placeholder (no unit)</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      metric unit of volume
    </th>
  </tr>
  <tr>
    <td><b>ml</b></td><td>milliliter (0.001 liter)</td>
  </tr>
  <tr>
    <td><b>cl</b></td><td>centiliter (0.01 liter)</td>
  </tr>
  <tr>
    <td><b>dl</b></td><td>deciliter (0.1 liter)</td>
  </tr>
  <tr>
    <td><b>l </b></td><td>liter</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      metric unit of weight
    </th>
  </tr>
  <tr>
    <td><b>mg</b></td><td>milligram (0.001 gram)</td>
  </tr>
  <tr>
    <td><b>cg</b></td><td>centigram (0.01 gram)</td>
  </tr>
  <tr>
    <td><b>dg</b></td><td>decigram (0.1 gram)</td>
  </tr>
  <tr>
    <td><b>g </b></td><td>gram</td>
  </tr>
  <tr>
    <td><b>kg</b></td><td>kilogram (1000 gram)</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      U.S. unit of volume
    </th>
  </tr>
  <tr>
    <td><b>fl</b></td><td>fluid ounce (1/128 gallon or 28.4130625 milliliter)</td>
  </tr>
  <tr>
    <td><b>pt</b></td><td>pint (1/8 gallon or 473.176473 milliliter)</td>
  </tr>
  <tr>
    <td><b>qt</b></td><td>quart (1/4 gallon or 0.946352946 liter)</td>
  </tr>
  <tr>
    <td><b>ga</b></td><td>gallon (3.785411784 liter)</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      U.S. unit of weight
    </th>
  </tr>
  <tr>
    <td><b>oz</b></td><td>ounce (1/16 pound or 28.34952312 gram)</td>
  </tr>
  <tr>
    <td><b>lb</b></td><td>pound (453.59237 gram)</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      unprecise unit of volume
    </th>
  </tr>
  <tr>
    <td><b>dr</b></td><td>drop (1/5 dash)</td>
  </tr>
  <tr>
    <td><b>ds</b></td><td>dash (1/2 pinch)</td>
  </tr>
  <tr>
    <td><b>pn</b></td><td>pinch (1/8 teaspoon)</td>
  </tr>
  <tr>
    <td><b>ts</b></td><td>teaspoon (1/6 fluid ounce or 4.928921595 milliliter)</td>
  </tr>
  <tr>
    <td><b>tb</b></td><td>tablespoon (1/2 fluid ounce or 14.7867647825 milliliter)</td>
  </tr>
  <tr>
    <td><b>c </b></td><td>cup (8 fluid ounces or 236.5882365 milliliter)</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      unprecise description of size
    </th>
  </tr>
  <tr>
    <td><b>sm</b></td><td>small</td>
  </tr>
  <tr>
    <td><b>md</b></td><td>medium</td>
  </tr>
  <tr>
    <td><b>lg</b></td><td>large</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      unprecise description of quantity
    </th>
  </tr>
  <tr>
    <td><b>cn</b></td><td>can</td>
  </tr>
  <tr>
    <td><b>pk</b></td><td>package</td>
  </tr>
  <tr>
    <td><b>ct</b></td><td>carton</td>
  </tr>
  <tr>
    <td><b>sl</b></td><td>slice</td>
  </tr>
  <tr>
    <td><b>bn</b></td><td>bunch</td>
  </tr>
  <tr>
    <th colspan="2" style="text-align: center; background-color: #DDDDBB">
      alternate tokens
    </th>
  </tr>
  <tr>
    <td><b>ea</b></td><td>"each" (deprecated placeholder)</td>
  </tr>
  <tr>
    <td><b>cb</b></td><td>cubic centimeter (1 milliliter)</td>
  </tr>
  <tr>
    <td><b>t </b></td><td>teaspoon</td>
  </tr>
  <tr>
    <td><b>T </b></td><td>tablespoon</td>
  </tr>
</table>

<h4>One-column format</h4>

Below follows an example of the one-column MealMaster format (the ingredients are listed in a single column).

```
MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------
     Title: Fruit-Meringue-Pie
Categories: pastries,cakes
  Servings: 1 pie

MMMMM----------------------------sponge mixture-----------------------------
    150 g  soft butter
    150 g  sugar
      3    eggs
    150 g  flour
      1 ts baking powder
MMMMM-------------------------------meringue--------------------------------
      4    egg white
    150 g  sugar
     50 g  almond leaves
MMMMM-------------------------------filling---------------------------------
    370 g  sour cherries;dripped off (
           -1 glass)
  1 1/2 tb cornflour
    1/4 l  cherry juice
    250 g  cream
      1 tb sugar
      1 pk stabiliser

Butter spring form with butter paper. Preheat oven to 200°.

MMMMM----------------------------sponge mixture-----------------------------
Stir butter, sugar and eggs until foamy, mix flour and baking powder and stir
in. Fill dough in in two spring forms and smooth down. Pre-bake for 15
minutes.

MMMMM-------------------------------meringue--------------------------------
Beat egg white until stiff and let sugar trickle in spoon by spoon. Beat
until stiff. Spread meringue base on pre-backed flan bases. Scatter almond
leaves over them. Bake for further 10-15 minutes. Let flan bases cool down on
a grid.

MMMMM-------------------------------filling---------------------------------
For the filling mix cornflour with some cherry juice. Bring remaining juice
to boiling point, stir in cornflour and boil up. Mix in sour cherries, let
cool down a bit and spread over one flan base.

Beat cream with sugar and stabiliser until stiff, spread over filling.
Cut second flan base in 12 pieces and reassemble on top of the filling. Let
cool down well.

MMMMM
```

The recipe starts with a title of up to 60 characters.
Then a list of categories follows.
Each category can have up to 11 characters.
Then servings amount and unit are specified.

In this recipe there are three ingredient sections.
An ingredient section starts with MMMMM or -----.
Ingredient amounts are specified in column 1 to 7.
Column 9 and 10 specify the unit token.
Starting from column 12 ingredient text of up to 28 characters follows.
If the ingredient text is longer than 28 characters, it is broken up using ingredient continuation lines.
Ingredient continuation lines begin with a "-" in column 12.

Finally the instruction text follows.
The instruction text also can be split up into multiple sections starting with MMMMM or -----.
The recipe ends with MMMMM.

<h4>Two-column format</h4>

Here is an example of the two-column format.
The ingredients are listed in two columns.

```
---------- Recipe via Meal-Master (tm) v8.01

      Title: Cannoli
 Categories: Italian, Desserts
      Yield: 16 servings

----------------------------------FILLING----------------------------------
  1 1/2 c  Whole-milk ricotta cheese;      1 1/2 c  Milk chocolate;
           - well drained                           - coarsely chopped
      3 tb Sugar                             1/4 c  Pistachio nuts;
  1 1/2 ts Cinnamon                                 - coarsely chopped

-----------------------------------DOUGH-----------------------------------
      1 c  All-purpose flour                        - or dry white wine
      1 tb Sugar                               2 c  Vegetable oil
      1 tb Butter or lard                           Colored sprinkles
      4 tb To 5 Tbl sweet Marsala wine

  In a bowl, combine all the filling ingredients and mix well. Refreigerate,
  covered, until ready to fill the cannoli shells.

  To make the dough, place the flour in a bowl or food processor. Add the
  butter or lard and sugar and mix with a fork, or pulse, until the mixture
  resembles coarse meal. Slowly add the 1/4 cup of wine and shape the mixture
  into a ball; add a little more wine if the dough appears too dry. It should
  be soft but not sticky. Knead the dough on a floured surface until smooth,
  about 10 minutes.  Wrap the dough and refrigerate for 45 minutes.

  Place the chilled dough on a floured work surface. Divide the dough in
  half.  Work with 1 piece of dough at a time; keep the remaining dough
  refrigerated. Roll the dough out to a very thin long rectangle about 14
  inches long and 3 inches wide, either by hand or using a pasta machine set
  to the finest setting.  Cut the dough into 3-inch squares. Place a cannoli
  form diagnoally across 1 square. Roll the dough up around the form so the
  points meet in the center.  Seal the points with a little water. Continue
  making cylinders until all the dough is used.

  In an electric skillet, heat the vegetable oil to 375F. Fry the cannoli 3
  or 4 at a time, turning them as they brown and blister, until golden brown
  on all sides. Drain them on brown paper. When they are cool enough to
  handle, carefully slide the cannoli off the forms.

  To serve, use a long iced tea spoon or a pastry bag without a tip to fill
  the cannoli with the ricotta cheese mixture. Dip the ends into colored
  sprinkles, arrange them on a tray, and sprinkle confectioner's sugar over
  the tops.  Serve at once.

  NOTE:  If you prefer, you can fry the cannoli in a deep fryer. Be sure to
  fill the cannoli just before serving - any sooner will make the shells
  soggy.

  This recipe from CIAO ITALIA by Mary Ann Esposito

-----
```

Here "Yield" is used instead of "Servings".
Note that the order of ingredients in each section is column major.
In this example the second ingredient section has a second column starting with an ingredient continuation line.
I.e. it is continuing the last ingredient of the first column.

This example also shows instruction text being indented by two spaces.
Empty lines are used to create paragraphs.

The recipe ends with -----.

Not shown in these examples is to use an instruction line beginning with a ":" to force a line break.

<h4>Conclusion</h4>

The MealMaster file format is quite difficult to parse.
Also there are a few problems with the file format:

* It is not possible to protect a comma in a category-name from interpretation as a separator.
* To prevent an occurrence of -----, : or MMMMM in the instructions-text from being interpreted as a special token, some recipe applications prepend all instruction lines with two space characters, which need to be ignored during import.
* The original Mealmaster software will refuse to import recipes exceeding a certain size. Also overlong titles, categories, and servings will be cropped.
* There are many recipes with ingredient-text longer than 28 characters. Overlong ingredient lines are making it more difficult to distinguish between the one- and two-column format.
* The ending of the ingredient section is not defined properly.
* Some recipe applications are relying on an empty line separating the ingredients from the instructions. On the other hand the original Mealmaster application allows empty lines within the ingredient-section.

I hope this description will be useful to you if you are trying to parse MealMaster recipes.
Please let me know any feedback in the comment section below.

[1]: https://web.archive.org/web/20081221021301/http://episoft.home.comcast.net/~episoft/mmdown.htm
[2]: http://www.faqs.org/rfcs/rfc2234.html
