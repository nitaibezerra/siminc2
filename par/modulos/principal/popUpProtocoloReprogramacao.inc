<?php


?>

<html>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
  <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
    <script language="javascript">$1_11 = $.noConflict();</script>

</head>




<body marginwidth="0" marginheight="0" bgcolor="#ffffff" leftmargin="0" topmargin="0">

<?php 
$sql = "
    		SELECT 
				hsrprotocolo,
				to_char(hsrdata, 'DD/MM/YYYY HH24:MI') as data_solicitacao,
				usucpf || ' - ' || usunome as nome_usuario,
				CASE WHEN hsrtipo = 'S' THEN
					array_to_string( 
						array(SELECT par.retornacodigosubacao(s.sbaid) || ' - ' ||  sbadsc  FROM par.historicosolicitacaoreprogramacaosubacao hrs
							INNER JOIN par.subacao s ON s.sbaid = hrs.sbaid where hsr.hsrid = hrs.hsrid )
							, '</br>' 
					) 
				ELSE
					''
				END
				as subacoes,
				CASE WHEN hsrtipo = 'S' THEN
					'Subação'
				ELSE
					'Prazo'
				END AS tipo,
				(SELECT
					CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
				ELSE
					dp.dopnumerodocumento::text
				END
					as ndocumento
				FROM 
					par.documentopar dp
			
				LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
				WHERE dp.dopid = hsr.dopid) as numero_doc,
				hsrtipo,
				
				hsrjustificativa as justificativa
			
			FROM par.historicosolicitacaoreprogramacao hsr
			INNER JOIN seguranca.usuario u ON hsr.hsrcpf = u.usucpf where  hsr.hsrid = {$_REQUEST['hsrid']}
";

$dadosProtocolo = $db->carregar($sql);

if( ! $dadosProtocolo)
{
	die('Erro ao localizar dados');
}
$dadosProtocolo = (is_array($dadosProtocolo[0])) ? $dadosProtocolo[0] : Array();

?>
<br>
<center><a onclick="window.print(); return false;" title="Versão para impressão"
							style="cursor: pointer;"> <img width="26px" height="26px" title="Versão para impressão" src="/imagens/print_g.png"></a></center><br>
<?php

monta_titulo( "Histórico de Reprogramação/Prorrogação", "" );



?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- JAVASCRIPT -->
<script>


jQuery(document).ready(function(){


	

});


</script>

<?php 

// /onclick="self.close();
// <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">


$tipo = $dadosProtocolo['hsrtipo'];

if($tipo == 'S' )
{
	$colunaTituloSba = "
				<td>
					<b>Subações</b>
				</td>
		";
	$colunaSba = "
				<td>
					{$dadosProtocolo['subacoes']}
				</td>	
						";
	$tituloTabela = "Solicitação de Reprogramação de Subação";
}
else
{
	$colunaTituloSba = "";
	$colunaSba = "";
	$tituloTabela = "Solicitação de Prorrogação de Prazo ";
}



if( ! count($dadosProtocolo))
{
	return false;
}
		
	$strMensagem = "<p style=\"text-align: justify;\">
	
	<b><center> {$tituloTabela}</center></b><center>
		<table border=\"1\" cellspacing=\"0\" cellpadding=\"1\">
			<tr>
				<td>
					<b>Data/Hora</b>
				</td>
				<td>
					<b>CPF/Nome Solicitante</b>
				</td>
				<td>
					<b>Tipo</b>
				</td>
				<td>
					<b>Nº Documento</b>
				</td>
				{$colunaTituloSba}
				<td>
					<b>Justificativa</b>
				</td>
			</tr>
				
			<tr>
				<td>
					{$dadosProtocolo['data_solicitacao']}
				</td>
				<td>
					{$dadosProtocolo['nome_usuario']}
				</td>
				<td>
					{$dadosProtocolo['tipo']}
				</td>
				<td>
					{$dadosProtocolo['numero_doc']}
				</td>
				{$colunaSba}
				<td>
					{$dadosProtocolo['justificativa']}
				</td>
			</tr>
		</table>
	</center>
		</p>";


		
		?>
					
					
						<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
							<tr >
								<td align="center">

	<div  style='width: 700; 
	 		min-height:200;
	 		background-color: white;
	 		padding: 10px;
	 		
	 		'
	 	  id="div_termo"
	  >

<center>
       <br>
<b style="font-size: 15">Protocolo <?=$dadosProtocolo['hsrprotocolo']?> </b>
</center>
<br><br><br>
<p style="font: 10pt Arial,verdana;  text-align: justify !important; " >
	<?= $strMensagem ?>

<br><br>
					
					
				
						</table>
				
					<?php
	

die();

$cpfNutricionista = $_SESSION['usucpf'];

?>

 
 
</div>
