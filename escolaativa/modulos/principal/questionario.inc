<html>
  <head>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
    <script	type="text/javascript" src="../includes/dtree/dtree.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
  </head>
<br>    
<body onunload="fechaPopup()">
<?php
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

extract($_GET);

$boVerSalvaQuestionario = 'N';

$qrpid = pegaQrpid( $tecid, 40, $monid );

if($qrpid){
	$sql = "SELECT
	        e.entnumcpfcnpj as cpf
	    FROM
	        entidade.entidade e
	    INNER JOIN escolaativa.tecnico t ON t.entid = e.entid
	    WHERE
	        t.tecid IN ( SELECT tecid FROM escolaativa.monitoramento WHERE qrpid = $qrpid )";
	$cpfQuestionario = $db->pegaUm($sql);
	if( ($cpfQuestionario == $_SESSION['usucpf'] && $_SESSION['escolaativa']['boPerfilMultiplicador']) || possuiPerfil(array(ESCOLAATIVA_PERFIL_SUPER_USUARIO)) ){
		$boVerSalvaQuestionario = 'S';		
	}
	
}

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Questionário';
monta_titulo($titulo_modulo,'Respondido<img src="/imagens/check_p.gif" style="border:0;">
							 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						     Não Respondido<img src="/imagens/atencao.png" style="border:0;">');

?>
<center>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td width="90%" >
			<fieldset style="width: 100%; background: #fff;"  >
				<legend>Questionário</legend>
				<?php
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivPx' => 355, "habilitado" => $boVerSalvaQuestionario) );
				?>
			</fieldset>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<?php
			if($_GET['monid']) {
				$bExiste = $db->pegaUm("SELECT count(1) FROM escolaativa.monitoramento WHERE monid = '{$_GET['monid']}' ");
				if($bExiste){
					$docid = eaPegarDocid($_GET['monid']);
					if($docid){ 
						wf_desenhaBarraNavegacao( $docid , array( 'monid' => $_GET['monid'] ) ); 
					}				
				}
			}
			?>
		</td>
	</tr>
</td>
</center>
<script type="text/javascript">
function fechaPopup(){
	var monid = '<?php echo $monid; ?>';
	var tipo = '<?php echo $_GET['tipo']; ?>';
	if(tipo == 'municipio'){
		var muncod = '<?php echo $_GET['muncod']; ?>';
		window.opener.location.href = 'escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A&monid='+monid+'&muncod='+muncod;	
	} else if(tipo == 'estuf'){
		var estuf = '<?php echo $_GET['estuf']; ?>';
		window.opener.location.href = 'escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A&estuf='+estuf;
	}
}
</script>
</body>
</html>