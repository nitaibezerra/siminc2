
<?php include APPRAIZ . 'includes/cabecalho.inc'; ?>

<link href="/sase/testes/jasmine/lib/jasmine.css" rel="stylesheet"/>
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script type="text/javascript" src="/sase/testes/jasmine/lib/jasmine.js"></script>
<script type="text/javascript" src="/sase/testes/jasmine/lib/jasmine-html.js"></script>
<script type="text/javascript" src="/sase/testes/jasmine/lib/boot/boot.js"></script>


<!--This file is not included in jasmine.js - but let's put it here for now -->
<script type="text/javascript" src="/sase/js/Mapas.js"></script>

<script type="text/javascript">
//shim for our tests using originalJasmine.createSpy and the like that should really be using globals defined in boot.
var originalJasmine = jasmine;
</script>

<!-- include spec files here... -->
<script type="text/javascript" src="/sase/testes/jasmine/spec/mapas/MapasSpec.js"></script>