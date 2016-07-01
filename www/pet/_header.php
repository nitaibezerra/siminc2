<html>
<head>
    <title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle</title>
    <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='/includes/loading.css'/>
    <link rel='stylesheet' type='text/css' href='/library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
    <link rel='stylesheet' type='text/css' href='/library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

    <!-- Force latest IE rendering engine or ChromeFrame if installed -->
    <!--[if IE]>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <![endif]-->
    <!--            <meta name="description" content="File Upload widget with multiple file selection, drag&amp;drop support, progress bars, validation and preview images, audio and video for jQuery. Supports cross-domain, chunked and resumable file uploads and client-side image resizing. Works with any server-side platform (PHP, Python, Ruby on Rails, Java, Node.js, Go etc.) that supports standard HTML form file uploads.">-->
    <!--            <meta name="viewport" content="width=device-width, initial-scale=1.0">-->
    <!-- Bootstrap styles -->
    <!--            <link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">-->
    <!-- Generic page styles -->
    <!--            <link rel="stylesheet" href="/library/bootstrap-file-upload-9.5.1/css/style.css">-->
    <!-- blueimp Gallery styles -->
    <!--    <link rel="stylesheet" href="http://blueimp.github.io/Gallery/css/blueimp-gallery.min.css">-->
    <!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
    <link rel="stylesheet" href="/library/bootstrap-3.0.0/css/bootstrap.css">

    <!-- jQuery JS -->
    <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="Javascript" src="/includes/funcoes.js"></script>
    <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <?php

    if ($theme && in_array($theme, $arrTheme))
        echo '<link href="/library/bootstrap-3.0.0/css/bootstrap-theme-' . $theme . '.css" rel="stylesheet" media="screen">';
    ?>
    <script src="/library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <!--<script src="js/html5shiv.js"></script>-->
    <!--<script src="js/respond.min.js"></script>-->
    <![endif]-->

    <!-- Chosen CSS -->
    <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen">
    <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen">

    <!-- Custom CSS -->
    <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">
    <link href="/library/simec/css/css_reset.css" rel="stylesheet">
    <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
</head>