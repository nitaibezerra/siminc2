<?php
ini_set("memory_limit", "2048M");


//ver($_REQUEST,d);

if($_REQUEST['atualizarFotosAjax']){
	$sql = "select fot.*, arq.arqdescricao from obras.fotos AS fot
						left join public.arquivo AS arq ON arq.arqid = fot.arqid
						where obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$_REQUEST["supvid"]." ORDER BY fotordem;";
				$fotos = ($db->carregar($sql)); 
	$sql = "SELECT
				arq.arqid
			FROM 
				public.arquivo arq
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			INNER JOIN 
				obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
			INNER JOIN 
				seguranca.usuario seg ON seg.usucpf = oar.usucpf 
			WHERE 
				obr.obrid = {$_SESSION["obra"]["obrid"]}
			AND
				aqostatus = 'A'
			AND
				(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
			ORDER BY 
				arq.arqid";
	$arrFotosGaleria = $db->carregar($sql);
	$arrArqidGaleria = array();
	if(is_array($arrFotosGaleria)){
		foreach($arrFotosGaleria as $foto){
			$arrArqidGaleria[] = $foto['arqid'];
		}
	}?>
	<?php if(is_array($fotos)): $arrFotos = $fotos;?>
		<?php $n = 1 ?>
		<?php foreach($arrFotos as $foto): ?>
			<?php $pagina = floor($n/16); ?>
			<li id="foto_<?php echo $foto['arqid']?>" class="draggable<?php echo in_array($foto['arqid'],$arrArqidGaleria) ? " f_selected" : ""?>">
				<img class="img_foto" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&_sisarquivo=obras&newwidth=100" />
			</li>
			<?php $n++ ?>
		<?php endforeach; ?>
	<?php endif;
	exit;
}

if($_REQUEST['atualizarDetalheParalisacao']){
    atualizarDetalheParalisacao($_REQUEST['tgpid']);
	exit;
}

function atualizarDetalheParalisacao($tgpid, $boAtivo='S'){
    global $db;
    $tgpid = $tgpid ? $tgpid : 0;

    $sql = "SELECT tplid as codigo, tpldsc as descricao
            FROM obras.tipoparalisacao
            where tgpid = $tgpid";

    $db->monta_combo("tplid", $sql, $boAtivo, "Selecione...", '', '', '', '', 'S', "tplid");
}

if($_REQUEST['obrid']) $_SESSION["obra"]['obrid'] = $_REQUEST['obrid']; 

if( !$_SESSION["obra"]['obrid'] || !$_SESSION["obra"]["orgid"] ){
      header( "location:obras.php?modulo=inicio&acao=A" );
      exit;
}

if( $_REQUEST['ajaxFotosGaleria'] && ($_REQUEST['fotosSupervisao'] || $_REQUEST['fotosGaleria']) ){
	if(atualizarFotosVistoria()){
		echo "true";
	}else{
		echo "false";
	}
	exit();
}

// busca as datas para validação da situação da vistoria;
$sql = "SELECT
			obrdtinicio,
			obrdttermino,
			dtiniciolicitacao,
			dtfinallicitacao,
			fprdtiniciofaseprojeto,
			fprdtconclusaofaseprojeto,
			stoid
		FROM
			obras.obrainfraestrutura oi
		LEFT JOIN
			obras.faseprojeto of ON of.obrid = oi.obrid
		WHERE
			oi.obrid = '".$_SESSION["obra"]["obrid"]."'";

$datasValidacao = $db->pegaLinha($sql);
$obraSituacaoGeral = $datasValidacao['stoid'];

obras_verificasessao();

$dado_situacao = obras_pega_situacao_vistoria( $_SESSION["obra"]["obrid"] );
$obridorigem   = obras_verifica_obra_copia( $_SESSION["obra"]["obrid"] );

// Comentado a pedido do Mario dia 19/06/2012
//if ( $dado_situacao == 3 && !$_REQUEST['supvid'] && !possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA,PERFIL_SUPERVISORUNIDADE ) ) ){
//	print '<script>alert("Não é possivel inserir uma nova vistoria!");history.back(-1);</script>';
//}
// FIM Comentado a pedido do Mario dia 19/06/2012

$boAtivo = ( $boAtivo == 'N' && !obraAditivoPossuiVistoria() ) ? 'S' : $boAtivo;
$habilitado     = ( !$habilitado && !obraAditivoPossuiVistoria() ) ? true : $habilitado;

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$obras = new Obras();
$dobras = new DadosObra(null);

// Realiza as ações do componente de fotos 
if($_REQUEST["AJAX"]){
	
	switch ($_REQUEST["subacao"]){
		case "ShowImage":
			$obras->ShowImage( $_REQUEST["img"], $_REQUEST["dir"] );
		break;
		case "UpdateListFoto":
			$obras->UpdateListFoto();
		break;
		case "FotosVistoria":
			$obras->AtualizarFotosVistoria( $_REQUEST );
		break;
		case "editar":
			$obras->AtualizarFotosVistoria( $_REQUEST );
		break;
		case "deletar":
			$obras->DeletarFotoVistoria( $_REQUEST );
		break;
		case "galeria":
			$obras->EnviarGaleria( $_REQUEST["imagens"] );
		break;
		case "dataVistoria":
			$obras->verificarDataVistoria( $_REQUEST);
		break;
		case "itensVistoria":
			obras_listaitensvistoria_data_previa_chamada($_REQUEST);
		break;
	}
	
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$requisicao = 'cadastra';

if($_REQUEST["supvid"] && $_SESSION["obra"]["obrid"]){
		

	
	
	$boAtivo = obras_podeatualizarvistoria($_REQUEST["supvid"]);
	$requisicao = 'atualiza';
	$_SESSION["supvid"] = $_REQUEST["supvid"]; 
	$obrid = $_SESSION["obra"]["obrid"];
	$obridorigem = obras_verifica_obra_copia( $obrid );
	(empty($obridorigem))? $status = 'A' :  $status = 'I';
	$dados = $obras->Dados($obrid, $_SESSION["supvid"], $status);
	$dobras = new DadosObra($dados);	
	
	$sql = "SELECT
					s.supvid
				FROM
					obras.supervisao s
				WHERE
			 
				s.supvid = ( SELECT supvid as supvid 
						   FROM obras.supervisao 
						   WHERE supstatus = 'A' AND obrid = ".$obrid."
						   ORDER BY SUPVDT DESC, supvid DESC LIMIT 1)";
	
	$ultimaVistoria = $db->pegaUm($sql);
	
	if($ultimaVistoria != $_REQUEST["supvid"]){
		$boAtivo = "N";
	}
	
	$_SESSION["obrcustocontrato"] = $db->pegaUm("SELECT 
													 obrcustocontrato
												 FROM
													 obras.obrainfraestrutura
												 WHERE
													 obrid = {$_SESSION["obra"]["obrid"]}");

	// regra para verificar se há validação da obra e bloquear alteracao de supervisoes com datas inferiores
	if ($_SESSION["obras"]["orgid"] ==  ORGAO_FNDE)
	{
		global $boAtivo;
		$sql = "select supvdt from obras.supervisao where supvid = ".$_REQUEST["supvid"];
		$dado = $db->pegaLinha($sql);
		$supvdt = $dado['supvdt'];
			
		$sql = "select vlddtinclusaost25exec, vlddtinclusaost50exec from obras.validacao
		where obrid = {$_SESSION["obra"]["obrid"]}";
		$dado = $db->pegaLinha($sql);
	
		if ($dado['vlddtinclusaost25exec'])
		{
			if ($supvdt <= $dado['vlddtinclusaost25exec'])
			{
				$boAtivo = 'N';
			}
		}
		if ($dado['vlddtinclusaost50exec'])
		{
			if ($supvdt <= $dado['vlddtinclusaost50exec'])
			{
				$boAtivo = 'N';
			}
		}
	}
	//*********************************************

}else{
	$boAtivo = 'S';
}

// Executa as funções da tela de acordo com suas ações
switch ($_REQUEST["requisicao"]){
	case "cadastra":
		// Cadastra a vistoria 
		if(!$_REQUEST['obrid']) {
			
			die("<script>
					alert('Erro de parametros!');
					window.location='obras.php?modulo=principal/vistoria&acao=A';
				 </script>");
			
		}
		$obras->CadastrarVistoria( $_REQUEST );
	break;
	case "atualiza":
		if(!$_REQUEST['supvid']) {
			
			die("<script>
					alert('Erro de parametros!');
					window.location='obras.php?modulo=principal/vistoria&acao=A';
				 </script>");
			
		}
		// Atualiza a vistoria
		
		$obras->AtualizarVistoria( $_REQUEST );
		
	break;
	
}

/* Limpar a variavel de sessão quando for cadastro (não carregar fotos da supervisão da sessão) */

if($requisicao == 'cadastra') {
	unset($_SESSION["supvid"]);
}

?>
<br/>
<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Vistoria da Obra";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

// Pega a situação da obra
$situacao_obra = obras_pega_situacao_vistoria( $_SESSION["obra"]["obrid"] );


$alteracaoEmpresa = false;
//regra para permitir alteracao de dados da vistoria caso usuario com perfil de empresa,
//e o workflow esteja na situacao devolvido para empresa para ajustes.




if ($situacao_obra == FINALIZADA && $_REQUEST["supvid"])
{
	if ( possuiPerfil(PERFIL_EMPRESA) )
	{
		
		
		$esdid = recuperaSituacaoObra( $_SESSION['obra']['obrid'] );
		
		
		
		if ($esdid == OBRAAJUSTESUPERVISAO_EMPRESA || $esdid == OBRAREAJUSTESUPERVISAO_EMPRESA)
		{
			
			$sql = "select usucpf from obras.supervisao where supvid = ".$_REQUEST["supvid"];
			$rs = $db->pegaUm($sql);
			
			if($rs ==  $_SESSION['usucpf'] )
			{
				$habilitado = true;
				$alteracaoEmpresa = true;
				$boAtivo = 'S';
				
			}

		}
	}
	
}
else if ($situacao_obra == FINALIZADA)
{
	$inclusaoEmpresaObraFinalizada = false;
	//regra para permitir alteracao de dados da vistoria caso usuario com perfil de empresa,
	//e o workflow esteja na situacao devolvido para empresa para ajustes.
	
	if ( possuiPerfil(PERFIL_EMPRESA) )
	{
		$esdid = recuperaSituacaoObra( $_SESSION['obra']['obrid'] );

		if ($esdid == OBRAAJUSTESUPERVISAO_EMPRESA || $esdid == OBRAREAJUSTESUPERVISAO_EMPRESA)
		{
			$habilitado = true;
			$inclusaoEmpresaObraFinalizada = true;
			$boAtivo = 'S';
		}
	}
}



// Verifica ultimas vistorias realizadas por
$sql = "select 
            max(s.supvid) as supvid, 
            rs.rsudsc as suprealizacao
from 
            obras.supervisao s
left join 
            obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
where 
            s.obrid = {$_SESSION["obra"]["obrid"]}
and 
            s.supstatus = 'A'
and
            s.supvdt IN (select max(supvdt) from obras.supervisao s1 left join obras.realizacaosupervisao rs1 ON rs1.rsuid = s1.rsuid  where supstatus = 'A' and obrid =  s.obrid group by rs1.rsudsc)
group by 
            rs.rsudsc";

$rsVistorias = $db->carregar($sql);
$rsVistorias = $rsVistorias ? $rsVistorias : array();

$situacao = obras_pega_situacao( $_SESSION['obra']['obrid'] );
$boSituacaoBlo = array(4,5,7,9,99); #situação da obra que bloqueia os campos

foreach($rsVistorias as $rwVistoria){	
	
	$sql = "select usucpf from obras.supervisao where supvid = ".$rwVistoria['supvid'];
	$rsUsucpf = $db->pegaUm($sql);
	
	if($rwVistoria['supvid'] == $_REQUEST['supvid'] && $_SESSION['usucpf'] == $rsUsucpf /*&& !in_array($situacao, $boSituacaoBlo)*/ ){
		
		$rwVistoria['suprealizacao'] = trim($rwVistoria['suprealizacao']);
		
		if( $rwVistoria['suprealizacao'] == 'Empresa' && possuiPerfil(PERFIL_EMPRESA) ){
			$esdidTravaEmpresa = Array(OBRAAVALIACAOSUPERVISAO_MEC, OBRAREAVALIACAOSUPERVISAO_MEC);
			if( in_array($situacao,$esdidTravaEmpresa) ){
				$boAtivo = 'N';	
			}else{
				$boAtivo = 'S';	
			}
		}else
		if( $rwVistoria['suprealizacao'] == 'MEC' && possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR)) ){
			$boAtivo = 'S';	
		}else
		if( $rwVistoria['suprealizacao'] == 'Instituição' && possuiPerfil(PERFIL_SUPERVISORUNIDADE) ){
			$boAtivo = 'S';	
		}		
	}	
}

// ver($rwVistoria,'70187');
// $docid = obrPegaDocidObra( $_SESSION['obra']['obrid'] );
// $esdid = recuperaSituacaoObra( $_SESSION['obra']['obrid'] );

?>

<link rel="stylesheet" href="css/obras.css" type="text/css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
	var valor_antigo = new Array();

	
</script>
<script src="../includes/prototype.js"  ></script>
<script src="/obras/js/vistoria.js" 	></script>
<form id="formulario" name="formulario" method="post" action="<?php echo $caminho_atual; ?>acao=A">
	<?php 
	if ($inclusaoEmpresaObraFinalizada) 
		echo "<input type=hidden name=inclusaoEmpresaObraFinalizada id = inclusaoEmpresaObraFinalizada value = 1>";
	else
		echo "<input type=hidden name=inclusaoEmpresaObraFinalizada id = inclusaoEmpresaObraFinalizada value = 0>"
	?>
	<input type="hidden" name="comDataAnterior" 			  id="comDataAnterior""			 value="" />

	<?php if ($alteracaoEmpresa) {
		echo "<input type='hidden' name='alteracaoempresa' id='alteracaoempresa' value = '1'/>";
	}
	?> 
	<input type="hidden" name="situacaoatual" 			  id="situacaoatual" 			 value="<?php echo ($situacao_obra ? $situacao_obra : $situacao); ?>" />

	<input type="hidden" name="situacao" 			  	  id="situacao" 			 	 value="<?php echo $situacao; ?>" />
	<input type="hidden" name="requisicao" 				  								 value="<?php echo $requisicao; ?>" />
	<input type="hidden" name="obrcustocontrato" 		  id="obrcustocontrato" 		 value="<?php echo $_SESSION["obrcustocontrato"]; ?>" />
	<input type="hidden" name="obrdtinicio" 			  id="obtdtinicio" 				 value="<?php echo formata_data( $datasValidacao["obrdtinicio"] ); ?>" />
	<input type="hidden" name="obrdttermino" 			  id="obrdttermino" 			 value="<?php echo formata_data( $datasValidacao["obrdttermino"] ); ?>" />
	<input type="hidden" name="dtiniciolicitacao" 		  id="dtiniciolicitacao"   		 value="<?php echo formata_data( $datasValidacao["dtiniciolicitacao"] ); ?>" />
	<input type="hidden" name="dtfinallicitacao"          id="dtfinallicitacao" 		 value="<?php echo formata_data( $datasValidacao["dtfinallicitacao"] ); ?>" />
	<input type="hidden" name="fprdtiniciofaseprojeto"    id="fprdtiniciofaseprojeto" 	 value="<?php echo formata_data( $datasValidacao["fprdtiniciofaseprojeto"] ); ?>" />
	<input type="hidden" name="fprdtconclusaofaseprojeto" id="fprdtconclusaofaseprojeto" value="<?php echo formata_data( $datasValidacao["fprdtconclusaofaseprojeto"] ); ?>" />
	<input type="hidden" name="existesupervisaoinstituicao" id="existesupervisaoinstituicao" value="<?=$db->pegaUm("select count(supvid) from obras.supervisao where supstatus = 'A' and rsuid = 1 and obrid = ".$_SESSION["obra"]["obrid"]);?>">
	<input type="hidden" name="supvdtmax" id="supvdtmax" value="<?=$db->pegaUm("select max(supvdt) from obras.supervisao where supstatus = 'A' and obrid = ".$_SESSION["obra"]["obrid"]);?>">
	<input type="hidden" name="perfilinstituicao" id="perfilinstituicao" value="<?=possuiPerfil(PERFIL_SUPERVISORUNIDADE);?>">
	<input type="hidden" name="orgid" id="orgid" value="<?=$_SESSION["obra"]["orgid"];?>">
	
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>Dados da Vistoria</td>
		</tr>
<!--		<tr>-->
<!--			<td class="SubTituloDireita" width="190px;">Tipo de Vistoria</td>-->
<!--			<td>-->
				<?php
				
//					$sql = " SELECT
//								tpsid AS codigo,
//								tpsdsc AS descricao 
//							 FROM
//								obras.tiposupervisao";
//					$tpsid = $dobras->getTpsid();
//					$db->monta_combo("tpsid", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpsid');
				?>
<!--			</td>-->
<!--		</tr>		-->
		<tr>
			<td class="SubTituloDireita">Data da Vistoria</td>
			<td>
				<?php $supvdt = $dobras->getSupvDt();
				
				if(isset($_SERVER['HTTP_USER_AGENT'])){
				    $agente = $_SERVER['HTTP_USER_AGENT'];
				}
				
				if(strlen(strstr($agente,"Firefox")) > 0 ){ 
				
				  $browser = 'firefox';
				
				}
				
				if ($browser == 'firefox')
				{
				?>
				<?= campo_data2( 'supvdt', 'S', ($ultimaVistoria == $_REQUEST['supvid'] ? $boAtivo : 'N'), '', 'S','','' ,'','validaDataVistoria(true);','','supvdt' ); ?>
				<?php }else{ ?>
				<?= campo_data2( 'supvdt', 'S', ($ultimaVistoria == $_REQUEST['supvid'] ? $boAtivo : 'N'), '', 'S','','validaDataVistoria(true);' ,'','validaDataVistoria(true);','','supvdt' ); ?>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome do Vistoriador</td>
			<td>
				<?php
					$vistoriador = new Entidade($dobras->getSupVistoriador());
					$entnomevistoriador = $vistoriador->entnome;
					$entidvistoriador = $vistoriador->getPrimaryKey();
				?>
				<span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
			  	<input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<? if( isset($_SESSION["obra"]["obrid"]) ) echo $entidvistoriador; ?>">
			  	<?php if($boAtivo == 'S' && $entnomevistoriador == ''){?>
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
				<?php }?>
				<?php print obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação atual</td>
			<td>
				<?php
				if (!$alteracaoEmpresa)
				{
					
					
					if(!empty($obridorigem)) $dado_situacao = $dobras->getStoId();
					
					$sql = " SELECT
								ROUND(SUM(icopercexecutado), 2) 
							 FROM
								obras.itenscomposicaoobra 
							 WHERE 
							 	obrid = {$_SESSION["obra"]["obrid"]}";
				
					$percentual = $db->pegaUm( $sql );
					$stoid = $dado_situacao ? $dado_situacao : $dobras->getStoId();
//					$sql   = obras_situacao_possivel($obraSituacaoGeral);

					if($_SESSION["obra"]["orgid"]){
						
						$arrPerfil = pegaPerfilGeral();
						
						$filtro = '';
						if( $_SESSION["obra"]["orgid"] == 3 ){
							if( !in_array( PERFIL_ADMINISTRADOR, $arrPerfil ) && !in_array( PERFIL_SUPERUSUARIO, $arrPerfil ) ){
								if( in_array( PERFIL_SUPERVISORUNIDADE, $arrPerfil ) || in_array( PERFIL_SUPERVISORMEC, $arrPerfil ) || in_array( PERFIL_EMPRESA, $arrPerfil )  ) {
									$filtro = " and sto.stoid not in (10)";
								}else{
									$filtro = " and sto.stoid not in (2,10)";
								}
							}
						}
						
						//Condição para inserir vistorias com as situações atuais Paralisada e Cancelada
						if($filtro!=''){
							if($situacao_obra==2){
								$filtro='';
							}
							if($situacao_obra==10 || $obraSituacaoGeral==10){
								$filtro='';
							}
						}
						
						$sql = "SELECT
									sto.stoid as codigo, 
									sto.stodesc as descricao 
								FROM 
									obras.situacaoobra sto
								INNER JOIN obras.situacaoobraorgao soo ON soo.stoidpossivel = sto.stoid
								WHERE
									sto.stostatus = 'A'
								AND
									soo.stoidatual = $obraSituacaoGeral
								AND
									orgid = ".$_SESSION["obra"]["orgid"]." 
									$filtro
								ORDER BY
									stoordem";
						
	
						$habilSit = ($ultimaVistoria == $_REQUEST['supvid'] ? $boAtivo : 'N');
	
						if(!$_REQUEST['supvid'] && ($obraSituacaoGeral == '3' || $obraSituacaoGeral == '11') && ( !possuiPerfil(array(PERFIL_SUPERUSUARIO,PERFIL_ADMINISTRADOR)) ) ){ //obra concluida e perfil diferente de superusuario e administrador
							if ($inclusaoEmpresaObraFinalizada)
							{
								$habilSit = 'S';
								$stoid = $obraSituacaoGeral;
							}
							else 
							{
								$habilSit = 'N';
								$stoid = $obraSituacaoGeral;
							}
							
						}
						
						$db->monta_combo("stoid", $sql, $habilSit, "Selecione...", "validasituacao(this.value,'1".possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORUNIDADE))."');verificasituacao", '', '', '', 'S', 'stoid');
					}
				
				}
				else {
					$sql = "select s.stoid as codigo, stodesc as descricao from obras.supervisao s
					inner join obras.situacaoobra so on s.stoid = so.stoid 
					where supvid =". $_REQUEST["supvid"];
					
					$db->monta_combo("stoid", $sql, 'N',"", "validasituacao(this.value,'1".possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORUNIDADE))."');verificasituacao", '', '', '', 'S', 'stoid');
					$d = $db->pegaLinha($sql);
				
					 $stoid = $d['codigo'];
}
				
					$msgParalisacao = "Favor preencher a aba <b>Restrições e Providências</b> com as informações sobre a paralisação.";
					if($_SESSION["obra"]["orgid"] == 3) $msgParalisacao = "";
					?>
				<span id="msg_paralisacao" style="display:none; color:#ee0000;"><?=$msgParalisacao?></span>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao">
			<td class="SubTituloDireita">Necessita de Licenciamento Ambiental</td>
			<td>
				<?php 
					$obrlincambiental = $dobras->getObrLincAmbiental(); 
					
					if( $_REQUEST["supvid"] ){
						if ($obrlincambiental == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao1">
			<td class="SubTituloDireita"> Necessita de aprovação junto ao patrimônio histórico</td>
			<td>
				<?php 
					$obraprovpatrhist = $dobras->getObrAprovPatrHist(); 
					
					if( $_REQUEST["supvid"] ){
						if ($obraprovpatrhist == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao2">
			<td class="SubTituloDireita">Previsão de entrega do(s) projeto(s)</td>
			<td>
				<?php $obrdtprevprojetos = $dobras->getObrDtPrevProjetos(); ?>
				<?= campo_data2( 'obrdtprevprojetos', 'S', $boAtivo, '', 'S','','' ); ?>
			</td>
		</tr>
        <tr style="display: none;" id="tr_tgpid">
            <td class="SubTituloDireita">Tipo de Paralisação</td>
            <td>
                <?php
                $tplid = $dobras->getTplId();
                $tplid = $tplid ? $tplid : 0;
                $sql = "SELECT tgpid FROM obras.tipoparalisacao WHERE tplid = {$tplid}";
                $tgpid = $db->pegaUm($sql);
                $sql = "SELECT
								tgpid as codigo,
								tgpdsc as descricao
							FROM
								obras.tipogrupoparalisacao";
                $db->monta_combo("tgpid", $sql, $boAtivo, "Selecione...", "", '', '', '', 'S', "tgpid","");
                ?>
            </td>
        </tr>
		<tr style="display: none;" id="tr_tplid">
			<td class="SubTituloDireita">Detalhe da Paralisação</td>
			<td>
                <span id="span-detalhe-paralisacao">
                    <?php atualizarDetalheParalisacao($tgpid, $boAtivo) ?>
                </span>
			</td>
		</tr>
		<tr style="display: none;" id="tr_hprobs">
			<td class="SubTituloDireita">Observações da Paralisação</td>
			<td>
				<?php
					$hprobs1 = $dobras->getHprObs();
					echo campo_textarea( 'hprobs1', 'N', 'S', '', '70', '8', '1000', '' , 0, '', '', false, '', ''); 
				?>
			</td>
		</tr>	
		<?php //if( !in_array($situacao, $boSituacaoBlo) ): ?>	
			<tr id="tr1" style="display: none;">
				<td class="SubTituloDireita">Projeto/Especificações</td>
				<td>
					<?php
					 
						if($boAtivo != 'S'){
							$disabled ="disabled"; 
						}
						$supprojespecificacoes = $dobras->getSupProjEspecificacoes(); 
						
						if($_REQUEST["supvid"]){
							if ($supprojespecificacoes == "t"){
								echo "<input type=\"radio\" checked=\"checked\" name=\"supprojespecificacoes\" value=\"1\"> Sim";
								echo "<input type=\"radio\" name=\"supprojespecificacoes\" value=\"0\"> Não";
							} else {
								echo "<input type=\"radio\" name=\"supprojespecificacoes\" value=\"1\"> Sim";
								echo "<input type=\"radio\" checked=\"checked\" name=\"supprojespecificacoes\" value=\"0\"> Não";
							}	
						}else{
							echo "<input type=\"radio\"  name=\"supprojespecificacoes\" value=\"1\"> Sim";
							echo "<input type=\"radio\"  checked=\"checked\" name=\"supprojespecificacoes\" value=\"0\"> Não";
						}
					?>
				</td>
			</tr>
			<tr id="tr2" style="display: none;">
				<td class="SubTituloDireita">Placa da Obra</td>
				<td>
					<?php 
						$supplacaobra = $dobras->getSupPlacaObra(); 
						
						if($_REQUEST["supvid"]){
							if ($supplacaobra == "t"){
								echo "<input type=\"radio\"  checked=\"checked\" name=\"supplacaobra\" value=\"1\"> Sim";
								echo "<input type=\"radio\"  name=\"supplacaobra\" value=\"0\"> Não";
							} else {
								echo "<input type=\"radio\"  name=\"supplacaobra\" value=\"1\"> Sim";
								echo "<input type=\"radio\"  checked=\"checked\" name=\"supplacaobra\" value=\"0\"> Não";
							}	
						}else{
							echo "<input type=\"radio\"  name=\"supplacaobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\"  checked=\"checked\" name=\"supplacaobra\" value=\"0\"> Não";
						}
					?>
				</td>
			</tr>
			<tr id="tr3" style="display: none;">
				<td class="SubTituloDireita">Diário da Obra Atualizado</td>
				<td>
					<?php 
						$supdiarioobra = $dobras->getSupDiarioObra();
						
						if($_REQUEST["supvid"]){
							if ($supdiarioobra == "t"){
								echo "<input type=\"radio\"  checked=\"checked\" name=\"supdiarioobra\" value=\"1\"> Sim";
								echo "<input type=\"radio\"  name=\"supdiarioobra\" value=\"0\"> Não";
							} else {
								echo "<input type=\"radio\"  name=\"supdiarioobra\" value=\"1\"> Sim";
								echo "<input type=\"radio\"  checked=\"checked\" name=\"supdiarioobra\" value=\"0\"> Não";
							}	
						}else{
							echo "<input type=\"radio\" name=\"supdiarioobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"supdiarioobra\" value=\"0\"> Não";
						}
					?>
				</td>
			</tr>
			
			<!-- 
			
			<tr id="tr4" style="display: none;">
				<td class="SubTituloDireita">Placa Indicativa do <br /> Programa/Dados da obra</td>
				<td>
					<?php 
						$supplacalocalterreno = $dobras->getSupPlacaLocalTerreno();
	
						if($_REQUEST["supvid"]){
							if ($supplacalocalterreno == "t"){
								echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
								echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"0\"> Não";
							} else {
								echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
								echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"0\"> Não";
							}	
						}else{
							echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"0\"> Não";
						}
					?>
				</td>
			</tr>
			<tr id="tr5" style="display: none;">
				<td class="SubTituloDireita">Validade do Alvará da Obra</td>
				<td>
					<?php 
						$supvalidadealvara = $dobras->getSupValidadeAlvara();
						
						if($_REQUEST["supvid"]){
							if ($supvalidadealvara == "t"){
								echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"1\"> Sim";
								echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"0\"> Não";
							} else {
								echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"1\"> Sim";
								echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"0\"> Não";
							}	
						}else{
							echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"0\"> Não";
						}
					?>
				</td>
			</tr>
			
			-->
			
			<tr id="tr6" style="display: none;">
				<td class="SubTituloDireita">Qualidade de Execução da Obra/Projeto</td>
				<td>
					<?php
					$qlbid = $dobras->getQlbId();
					$sql = "SELECT 
								qlbid as codigo, 
								qlbdesc as descricao 
							FROM 
								obras.qualidadeobra"; 
					$db->monta_combo("qlbid", $sql, $boAtivo, "Selecione...", '', '', '', '100', 'S');
					
					?>
				</td>
			</tr>
			<tr id="tr7" style="display: none;">
				<td class="SubTituloDireita">Desempenho da Construtora/Projetista</td>
				<td>
					<?php
					$dcnid = $dobras->getDcnId();
					$sql = "SELECT 
								dcnid as codigo, 
								dcndesc as descricao 
							FROM 
								obras.desempenhoconstrutora"; 
					$db->monta_combo("dcnid", $sql, $boAtivo, "Selecione...", '', '', '', '100', 'S');
					
					?>
				</td>
			</tr>
		<?php //endif; ?>
		<tr id="tr8" style="display: none;">
			<td>Detalhamento de Supervisão e Acompanhamento</td>
		</tr>
		<tr id="tr9" style="display: none;">
			<td colspan="2" id="listaItensVistoria">
				<table class="listagem" width="100%">
					<thead>
						<tr>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Item da Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre a Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
							<td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
							<td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
						</tr>
						<tr>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
						</tr>
					</thead>
					<tbody>
						<?php
							$supvid = $_REQUEST["supvid"] ? $_REQUEST["supvid"] : 'NULL';
							
							// Função que lista os itens da supervisão.
							obras_listaitensvistoria( $supvid );
						?>
					</tbody>
				</table>
			</td>
		</tr>
		<tr>
			<!--<td class="SubTituloDireita">Observação da Vistoria</td>-->
			<td class="SubTituloDireita">Relatório Técnico da Vistoria</td>
			<td>
				<?php 
				$supobs = $dobras->getSupObs(); 
				$desc_relatorio = "No relatório deverão constar informações pertinentes a execução da obra, tais como:<br/><br/>&nbsp;&nbsp;&nbsp;<b>a.</b> O nome do Responsável Técnico da obra e o número da ART de execução.<br/>&nbsp;&nbsp;&nbsp;<b>b.</b> A existência ou não de todos os documentos necessários para a execução da obra, tais como: projetos, especificações, memoriais, ART´s, licenças, alvarás, caderno de encargos, diários de obras (preenchido e atualizado), dentre outros.<br/>&nbsp;&nbsp;&nbsp;<b>c.</b> Informações sobre o canteiro de obra (existência, organização e segurança do Trabalho).<br/>&nbsp;&nbsp;&nbsp;<b>d.</b> Atrasos na execução da obra e a necessidade ou não de aditivos.<br/>&nbsp;&nbsp;&nbsp;<b>e.</b> Comentário sobre a situação geral da obra e/ou de cada serviço da obra.<br/>&nbsp;&nbsp;&nbsp;<b>f.</b> Outros apontamentos.";
				?>
				<?= campo_textarea( 'supobs', 'S', $boAtivo, '', '100', '8', '10000','','',$desc_relatorio); ?>
			</td>
		</tr>
		<?php
			if($_REQUEST["supvid"]){
				$sql = "select fot.*, arq.arqdescricao from obras.fotos AS fot
						left join public.arquivo AS arq ON arq.arqid = fot.arqid
						where obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$_REQUEST["supvid"]." ORDER BY fotordem;";
				$fotos = ($db->carregar($sql));
			}
		?>
		<tr id="tr10" style="display: none;">
			<td colspan="2" align="center">
			</td>
		</tr>
	</table>
	<input type="hidden" name="supvid" value="<?php echo $_REQUEST["supvid"]; ?>"/>
	<input type="hidden" name="ultimavistoria" value="<?php echo $ultimaVistoria; ?>"/>
	<input type="hidden" name="obrid" value="<? echo $_SESSION["obra"]["obrid"]; ?>">
	<input type="hidden" name="percexec" value="<? echo $percentual; ?>">
	<input type="hidden" name="totalvalor" id="totalvalor" value="<?php echo $_SESSION["obras"]["totalvalor"]; ?>" />
	<input type="hidden" name="hdn_fotos_galeria" id="hdn_fotos_galeria" value="" />
	<input type="hidden" name="hdn_fotos_supervisao" id="hdn_fotos_supervisao" value="" />
</form>


<?php if ( $dobras->getStoId() == PARALIZADA ){ ?>
		
	<script>
		if (document.selection){
			document.getElementById('tr_tplid').style.display = 'block';
			document.getElementById('tr_hprobs').style.display = 'block';
            document.getElementById('tr_tgpid').style.display = 'block';
		}else{
			document.getElementById('tr_tplid').style.display = 'table-row';
			document.getElementById('tr_hprobs').style.display = 'table-row';
            document.getElementById('tr_tgpid').style.display = 'table-row';
		}
	</script>
		
<?php }else if ( $dobras->getStoId() == EM_ELABORACAO_DE_PROJETOS ) { ?>

	<script>
		if (document.selection){
			document.getElementById('tr_elaboracao').style.display  = 'block';
			document.getElementById('tr_elaboracao1').style.display = 'block';
			document.getElementById('tr_elaboracao2').style.display = 'block';
		}else{
			document.getElementById('tr_elaboracao').style.display  = 'table-row';
			document.getElementById('tr_elaboracao1').style.display = 'table-row';
			document.getElementById('tr_elaboracao2').style.display = 'table-row';
		}
	</script>

<?php }


print "<script>verificasituacao(".($situacao_obra ? $situacao_obra : $situacao).");</script>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>
	jQuery.noConflict();

<?php if(possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE) )){ ?>
	jQuery(function() {
		jQuery( "#fotos_supervisao" ).sortable({
			placeholder: "draggable_space",
			connectWith: "#fotos_supervisao",
			cancel: ".nodraggable"
		});
		jQuery( "#fotos_galeria" ).droppable({
			drop: function( event, ui ) {
				if(!jQuery("#s_" + ui.draggable.attr("id")).html()){
					jQuery( "<li id=\"s_" + ui.draggable.attr("id") + "\" class=\"nodraggable\" ><img src=\"../imagens/fechar.jpeg\" title=\"Remover Foto\" class=\"fechar\" onclick=\"removerFotoGaleria('" + ui.draggable.attr("id") + "')\" />" + ui.draggable.html() + "</li>" ).appendTo( this );
					jQuery("#s_" + ui.draggable.attr("id") + " .img_foto" ).attr("style","margin-top:-15px");
					jQuery("#" + ui.draggable.attr("id") ).attr("class","draggable f_selected");

				}
			}
		}).sortable({
			cancel: ".nodraggable"
		});
		
	});

	function removerFotoGaleria(foto)
	{
		jQuery("#s_" + foto ).remove();
		jQuery("#" + foto ).attr("class","draggable");
	}
<?php } ?>
	
	function abrirGaleria(arqid,pagina)
	{
		window.open("../slideshow/slideshow/index.php?pagina=" + pagina + "&_sisarquivo=obras&arqid=" + arqid,"imagem","width=850,height=600,resizable=yes");
	}
	
	function salvarFotos()
	{
		jQuery("[name=btn_salvar_fotos]").val("Carregando...");
		jQuery("[name=btn_salvar_fotos]").attr("disabled","disabled");
		var arrFotosSupervisao = jQuery( "#fotos_supervisao").sortable( "serialize" , {key:'fotosSupervisao[]'} );
		var arrFotosGaleria    = jQuery( "#fotos_galeria").sortable( "serialize" , {key:'fotosGaleria[]'} );
		var url = window.location + "&ajaxFotosGaleria=true&" + arrFotosSupervisao + "&" + arrFotosGaleria;
		jQuery.ajax({
		  	url: url,
		  	success: function(data) {
		    if(data == "true"){
		    	alert("Operação realizada com sucesso!");
		    	jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
				jQuery("[name=btn_salvar_fotos]").attr("disabled","");
		    }else{
		    	alert("Não foi possível realizar a operação!");
		    	jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
				jQuery("[name=btn_salvar_fotos]").attr("disabled","");
		    }
		  }
		});
	}
	
</script>
<style>
	.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
	.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
	html>body .div_fotos li { height: 80px; line-height: 1.2em; }
	.field_fotos{padding:10px;}
	.div_fotos{height:300px;overflow:auto;}
	.draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
	.nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
	.f_selected{border: solid 1px red;}
	.fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
	.img_foto{z-index:2;}
	.img_class{margin-top:-15px;}
</style>
<?php 
// Verifica o usuário possui o Perfil: SUPERVISOR MEC ou ADMINISTRADOR, caso possua será habilitado o botão: "Salvar Fotos", para que as fotos sejam salvas.
( ( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR ) ) ) ? $permissao ='S' : $permissao ='N' );

$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");
$_SESSION['imgparams'] = array("filtro" => "cnt.obrid =".$_SESSION["obra"]["obrid"]." ".(($_REQUEST["supvid"])?"AND cnt.supvid=".$_REQUEST["supvid"]:""), "tabela" => "obras.fotos");

if(!$_REQUEST['supvid']){
	$sql = "SELECT
				arq.arqid
			FROM 
				public.arquivo arq
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			INNER JOIN 
				obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
			INNER JOIN 
				seguranca.usuario seg ON seg.usucpf = oar.usucpf
			INNER JOIN
				obras.supervisao supv ON supv.supvid = oar.obrid
			WHERE 
				obr.obrid = {$_SESSION["obra"]["obrid"]}
			AND
				aqostatus = 'A'
			AND
				tpaid = ".TIPO_ARQUIVO_FOTO_VISTORIA."
			AND
				(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
			ORDER BY 
				arq.arqid";
}else{
	$sql = "SELECT DISTINCT
				arq.arqid
			FROM 
				obras.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			WHERE 
				fot.obrid = {$_SESSION["obra"]["obrid"]}
				AND fot.supvid = {$_REQUEST['supvid']}
				AND oar.tpaid = ".TIPO_ARQUIVO_FOTO_VISTORIA."
				AND oar.aqostatus = 'A'
			ORDER BY 
				arq.arqid";
}
$arrFotosGaleria = $db->carregar($sql);
// array que armazenará o arqid da Galeria
$arrArqidGaleria = array();
if(is_array($arrFotosGaleria)){
	foreach($arrFotosGaleria as $foto){
		$arrArqidGaleria[] = $foto['arqid'];
	}
}
?>
<?php if( 1==1 ){  ?>
	<table class="tabela" id="tbl_graggable" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2">
				<b>Fotos de Vistoria</b>
				<?php if($_REQUEST["supvid"]): ?>
					*Arraste as imagens para a área de 'Fotos da Galeria' para adicioná-las à galeria. Após adicioná-las, clique no botão 'Salvar' para confirmar as alterações.
				<?php else: ?>
					*Após adicionar as fotos, clique no botão 'Salvar' para confirmar as alterações.
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td <?php echo $_REQUEST["supvid"]? "width=\"50%\"" : "colspan='2'" ?> >
				<fieldset class="field_fotos">
					<legend>Fotos da Supervisão</legend>
					<ul id="fotos_supervisao" class="div_fotos">
						<?php if(is_array($fotos)): $arrFotos = $fotos;?>
							<?php $n = 1 ?>
							<?php foreach($arrFotos as $foto): ?>
								<?php $pagina = floor($n/16); ?>
								
								<?php $sql = "SELECT arqtipo, arqid  FROM public.arquivo 
											WHERE arqid = '". $foto['arqid'] ."'";
									$dados = $db->pegaLinha($sql);
									
									$imgend = APPRAIZ.'arquivos/obras/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
									
									if(is_file($imgend)){
										$img_max_dimX = 100;
										$img_max_dimY = 85;
										
										$imginfo = getimagesize($imgend);
										
										$width = $imginfo[0];
										$height = $imginfo[1];
									
										if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
											if ($width > $height){
											  	$w = $width * 0.9;
												  while ($w > $img_max_dimX){
													  $w = $w * 0.9;
												  }
												  $w = round($w);
												  $h = ($w * $height)/$width;
											  }else{
												  $h = $height * 0.9;
												  while ($h > $img_max_dimY){
													  $h = $h * 0.9;
												  }
												  $h = round($h);
												  $w = ($h * $width)/$height;
											  }
										}else{
											  $w = $width;
											  $h = $height;
										}
										
										$tamanho = " width=\"$w\" height=\"$h\" ";
									}else{
										$tamanho = "";
									}
								//Permissão para arrastar as Fotos para a Galeria de Fotos.	
								(($permissao == 'S' && $boAtivo == 'S' || $permissao == 'S' )? $arrastar="draggable":$arrastar="nodraggable"); 
								?>
								<li id="foto_<?php echo $foto['arqid']?>" class="<?=$arrastar; echo in_array($foto['arqid'],$arrArqidGaleria) ? " f_selected" : ""?>">
									<img <?php echo $tamanho;?> class="img_foto" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&_sisarquivo=obras&newwidth=100&newheight=85" />
								</li>
								<?php $n++ ?>
							<?php endforeach; ?>
						<?php endif; ?>
					</ul>
				</fieldset>
			</td>
			<?php if($_REQUEST["supvid"]): ?>
				<td>
					<fieldset class="field_fotos">
						<legend>Fotos da Galeria</legend>
						<ul id="fotos_galeria" class="div_fotos">
							<?php if(is_array($arrFotosGaleria)):?>
								<?php $n = 1 ?>
								<?php foreach($arrFotosGaleria as $foto): ?>
									<?php $pagina = floor($n/16); ?>
									
									<?php $sql = "SELECT arqtipo, arqid  FROM public.arquivo 
												WHERE arqid = '". $foto['arqid'] ."'";
										$dados = $db->pegaLinha($sql);
										
										$imgend = APPRAIZ.'arquivos/obras/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
										
										if(is_file($imgend)){
											$img_max_dimX = 100;
											$img_max_dimY = 85;
											
											$imginfo = getimagesize($imgend);
											
											$width = $imginfo[0];
											$height = $imginfo[1];
										
											if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
												if ($width > $height){
												  	$w = $width * 0.9;
													  while ($w > $img_max_dimX){
														  $w = $w * 0.9;
													  }
													  $w = round($w);
													  $h = ($w * $height)/$width;
												  }else{
													  $h = $height * 0.9;
													  while ($h > $img_max_dimY){
														  $h = $h * 0.9;
													  }
													  $h = round($h);
													  $w = ($h * $width)/$height;
												  }
											}else{
												  $w = $width;
												  $h = $height;
											}
											
											$tamanho = " width=\"$w\" height=\"$h\" ";
										}else{
											$tamanho = "";
										}
									?>
									
									<li id="s_foto_<?php echo $foto['arqid']?>" class="nodraggable">
										<?if($boAtivo == 'S' || $permissao =='S'){ ?>
										<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoGaleria('foto_<?php echo $foto['arqid'] ?>')" />
										<?php }else{?>
										<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar"/>
										<?php }?>
										<img <?php echo $tamanho;?> class="img_class" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&_sisarquivo=obras&newwidth=100&newheight=85" />
									</li>
									<?php $n++ ?>
								<?php endforeach; ?>
							<?php endif; ?>
						</ul>
					</fieldset>
				</td>
			<?php endif; ?>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0" colspan="2" align="center">
				<?php //if($habilitado && $boAtivo == 'S' || possuiPerfil(PERFIL_SUPERVISORMEC)){ ?>
					<input <?=(($habilitado && $boAtivo == 'S')? "" :"disabled=\"disabled\""); ?> type="button" name="add_fotos" onmouseover="return escape('Gerenciamento de fotos da vistoria (inserir, alterar e remover).');" value="Adicionar Fotos" onclick="ImageComponent(<? if(isset($_REQUEST['supvid'])){ echo "'?funcao=AtualizaFotos&supvid=".$_REQUEST["supvid"]."'";}else{ echo "' '";}?>);">
					<?php //if(1==2): ?>
					<?php //if($_REQUEST['supvid']): ?>
					<input <?=(( $habilitado && $boAtivo == 'S' || $permissao == 'S')? ($boBloqueioAba ? "disabled=\"disabled\"" : "" ) :"disabled=\"disabled\""); ?> type="button" name="btn_salvar_fotos" onmouseover="return escape('Salvar as alterações das fotos de supervisão e galeria.');" value="Salvar Fotos" onclick="salvarFotos()">
					<?php //endif; ?>
				<?php //} ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0" colspan="2" align="center">
					<?php
					
						$disableVistoria = '';
						
						if(!$habilitado || $boAtivo == 'N' /*|| possuiPerfil(PERFIL_SUPERVISORMEC)*/){ 
							$disableVistoria = 'disabled="disabled"';
						}
						
						//LIBERAÇÃO TEMPORÁRIA A PEDIDO DA KENYA DIA 28/06/2013
						if($_SESSION['usucpf']==''){
							if($_REQUEST['supvid']==123532||$_REQUEST['supvid']==137013||$_REQUEST['supvid']==139188||$_REQUEST['supvid']==136941||$_REQUEST['supvid']==138051){
							$disableVistoria = '';
							}
						}	
						?> 
						<input type="button" <?=$disableVistoria; ?> value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="validaDataVistoria(false); enviaFormulario('1<?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO /*,PERFIL_SUPERVISORUNIDADE,PERFIL_EMPRESA*/));?>');">
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>
	</table>
<?php } ?>
<?php if($boAtivo == 'S'): ?>
	<script>
	jQuery(function(){
		//jQuery("[name^='percexecsobreobra_']").attr('readonly', false).removeClass('disabled').addClass('normal');
		jQuery("[name^='supvlrinfsuperivisor_']").attr('readonly', false).removeClass('disabled').addClass('normal');

        jQuery('#tgpid').change(function(){
            jQuery('#span-detalhe-paralisacao').load('?modulo=principal/inserir_vistoria&acao=A&atualizarDetalheParalisacao=1&tgpid='+jQuery(this).val());
        });

	});
	</script>
<?php endif; ?>