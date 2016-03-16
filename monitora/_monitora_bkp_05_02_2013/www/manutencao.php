<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   MÃ³dulo:inclusao_usuario.INC
   Finalidade: permitir o controle de cadastro de usuários do simec
   */
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
?>
<title>Manutenção do SIMEC</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<body>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<? include "cabecalho.php";

?>
<br>
<?
$titulo_modulo='O Sistema SIMEC está temporariamente em manutenção';
$subtitulo_modulo='Por favor, aguarde alguns minutos.<br>';
monta_titulo($titulo_modulo,$subtitulo_modulo);
?>



</body>
</html>
