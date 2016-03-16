<?php
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ . "emenda/classes/LogErroWS.class.inc";
require_once APPRAIZ . "emenda/classes/Habilita.class.inc";


$_SESSION["emenda"]["ptrid"] = !empty( $_REQUEST["ptrid"] ) ? $_REQUEST["ptrid"] : $_SESSION["emenda"]["ptrid"];
$ptrid = $_SESSION["emenda"]["ptrid"];

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A";
		  </script>';
	die;
}

include_once APPRAIZ . 'includes/workflow.php';
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}

print "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

// Cria o título da tela
//monta_titulo( 'Execução do PTA', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");
monta_titulo( 'Consulta Habilitação', "");

echo cabecalhoPlanoTrabalho($ptrid);
$obHabilita = new Habilita();
$cnpj = $obHabilita->pegaCnpj($ptrid);

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

if(  in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ) {
	$sql = "SELECT DISTINCT
			    count(ini.iniobras)
			FROM
				emenda.planotrabalho ptr
			    inner join emenda.entidadebeneficiada ent on ent.enbid = ptr.enbid
			    inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
			    inner join emenda.iniciativa ini on ini.iniid = pti.iniid    
			WHERE
			    ptr.ptrstatus = 'A' 
			    AND ptr.ptrexercicio = 2010
			    and ini.iniobras = 'S'
			    and ptr.ptrid = $ptrid
			    AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')";
	if($db->pegaUm( $sql ) == 0){
		$html = '<tr>
					<td>Senhores dirigentes, \nInformamos que a efetivação do convênio dar-se-á mediante a apresentação da documentação de comprovação de dominialidade.</td>
				</tr>';
	}
}
?>
<html>
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<link rel="stylesheet" type="text/css" href="css/emenda.css"/>
<body>
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<?=$html; ?>
		<tr>
			<td><a href="http://www.fnde.gov.br/index.php/ph-arquivos/category/5-2011?download=57%3Ares02213052011">
				Resolução nº 22 de 13 de maio de 2011 – Estabelece os documentos necessários à certificação da situação de regularidade para transferência de recursos e habilitação de entidades. 
				</a></td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">Situação Habilitação</td>
		</tr>
		<tr>
			<td align="center"><?php echo $obHabilita->consultaHabilita($cnpj,true); ?></td>
		</tr>
	</table>
</body>
</html>