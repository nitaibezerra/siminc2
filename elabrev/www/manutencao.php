<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:inclusao_usuario.INC
   Finalidade: permitir o controle de cadastro de usu�rios do simec
   */
	  include "includes/classes_simec.inc";
      include "includes/funcoes.inc";
?>
<title>Manuten��o do <?php echo SIGLA_SISTEMA; ?></title>
<script language="JavaScript" src="includes/funcoes.js"></script>
<body>
<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<? include "cabecalho.php";

?>
<br>
<?php
$titulo_modulo='O Sistema '. SIGLA_SISTEMA. ' est� temporariamente em manuten��o';
$subtitulo_modulo='Por favor, aguarde alguns minutos.<br>';
monta_titulo($titulo_modulo,$subtitulo_modulo);
?>



</body>
</html>
