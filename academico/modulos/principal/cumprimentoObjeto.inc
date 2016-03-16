<?php
require_once APPRAIZ . "academico/classes/Processo.class.inc";
require_once APPRAIZ . "academico/classes/ProcessoObjeto.class.inc";
require_once APPRAIZ . "academico/classes/ProcessoNotaCredito.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "academico/classes/TermoCooperacao.class.inc";

unset($_SESSION['academico']['boPerfilSuperUsuario'],$_SESSION['academico']['boPodeCadastrar']);

# verifica se tem permissão de gravar
$habilitado = (possuiPerfilCadastro()) ? 'S' : 'N'; 

if( possuiPerfil(array(PERFIL_SUPERUSUARIO, PERFIL_MECCADASTRO) ) ){
	$_SESSION['academico']['boPerfilSuperUsuario'] = true;
} elseif( possuiPerfil(array(PERFIL_IFESCADBOLSAS, PERFIL_IFESCADCURSOS, PERFIL_IFESCADASTRO) ) ){
	$_SESSION['academico']['boPodeCadastrar'] = true;
}

$arUO = $arUG = array();
if($_SESSION['academico']['boPodeCadastrar']){
	
	$arUnidades = pegaUnidadeAssociada(pegaPerfil($_SESSION['usucpf']));
	//$arUnidades = pegaUnidadeAssociada(277);
	$arUnidades = ($arUnidades) ? $arUnidades : array();
	
	if(count($arUnidades)){
		foreach($arUnidades as $unidade){
			if($unidade['entunicod']){
				array_push($arUO, $unidade['entunicod']);				
			} elseif($unidade['entungcod']){
				array_push($arUG, $unidade['entungcod']);
			}
		}
	}	
}

$arUO = ($arUO) ? $arUO : array();
$arUG = ($arUG) ? $arUG : array();

if($_POST['acao']){
	extract($_POST);
	//ver($_POST,d);
	
	/*$prcnumero = str_replace(".", "", $prcnumero);
	$prcnumero = str_replace("/", "", $prcnumero);
	$prcnumero = str_replace("-", "", $prcnumero);*/
	$prcnumero = 'null';
	
	$obProcesso = new Processo();
	$obProcesso->prcid 			= $prcid;
	if($unicodexecutor){
		$obProcesso->unicodexecutor = $unicodexecutor;		
	}
	$obProcesso->unitpocod      = $unitpocod;
	if($ungcodgestor){
		$obProcesso->ungcodgestor 	= $ungcodgestor;
	}
	$obProcesso->prtid    		= $prtid;
	$obProcesso->prcnumero    	= $prcnumero;
	$obProcesso->prndtinicio    = $prndtinicio;
	$obProcesso->prndtfim    	= $prndtfim;
	$obProcesso->salvar();
	
	/*if($obProcesso->prcid && $pncnotacredito[0]){
		$obProcessoNotaCredito = new ProcessoNotaCredito();
		$obProcessoNotaCredito->excluirPorPrcid($obProcesso->prcid);
		$pncnotacredito = ($pncnotacredito) ? $pncnotacredito : array();
		foreach($pncnotacredito as $notacredito){
			$obProcessoNotaCredito->pncid 		  = null; 
			$obProcessoNotaCredito->prcid 		  = $obProcesso->prcid; 
			$obProcessoNotaCredito->pncnotacredito = $notacredito; 
			$obProcessoNotaCredito->pncdtinclusao  = date('Y-m-d H:m:s');
			$obProcessoNotaCredito->usucpf 		  = $_SESSION['usucpf'];
			$obProcessoNotaCredito->salvar();
			//ver($obProcessoNotaCredito);
		}		
	}*/
	
	$obProcessoObjeto = new ProcessoObjeto();
	$obProcessoObjeto->objid    		= $objid;
	$obProcessoObjeto->prcid 			= $obProcesso->prcid;
	$obProcessoObjeto->objdescricao     = $objdescricao;
	$obProcessoObjeto->objbeneficio     = $objbeneficio;
	$obProcessoObjeto->objjustificativa = $objjustificativa;
	$obProcessoObjeto->objplanotrabalho = $objplanotrabalho;
	if($objdata){
		$objdata = formata_data_sql($objdata);
	} else {
		$objdata = date('Y-m-d');
	}
	$obProcessoObjeto->objdata    		= $objdata;
	
	$obProcessoObjeto->objstatus    	= 'A';
	if(!$objid){
		$obProcessoObjeto->objdtinclusao = date('Y-m-d');		
	}
	$obProcessoObjeto->salvar();
	$boSucesso = $obProcessoObjeto->commit();
	if(!$boSucesso){
		$obProcessoObjeto->rollback();
	}
//	ver(123,d);
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cumprimentoObjeto","&prcid=".$obProcesso->prcid);
	unset($obProcessoObjeto);
	die;
}

if(!$_GET['prcid'] && $_SESSION['academico']['prcid']){
	$_GET['prcid'] = $_SESSION['academico']['prcid'];
}

$obProcesso = new Processo($_GET['prcid']);
$obProcessoObjeto = new ProcessoObjeto($obProcesso->prtid);

if($obProcesso->prcid){
	$_SESSION['academico']['prcid'] = $obProcesso->prcid;
} elseif($_GET['prcid']){
	$_SESSION['academico']['prcid'] = $_GET['prcid'];	
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

echo montarAbasArray( criaAbaPortaria(), "academico.php?modulo=principal/cumprimentoObjeto&acao=A" );

$titulo_modulo = "RELATÓRIO DE CUMPRIMENTO DO OBJETO";
monta_titulo( $titulo_modulo, 'Cumprimento do Objeto' );

$obData = new Data();
$obTermoCooperacao = new TermoCooperacao($obProcesso->tmcid);

include_once APPRAIZ . "includes/workflow.php";
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"><!--

$(document).ready(function() {

	$('#btnSalvar').click(function(){

		var msg = "";
		
		if($('#prtid').val() == ''){
			msg += "O campo Portaria N° é obrigatório.\n";
		}
		/*if($('#prcnumero').val() == ''){
			msg += "O campo N° do processo de concessão é obrigatório.\n";
		}*/
		if(trim($('#prndtinicio').val()) == ''){
			msg += "O campo Periodo de Execução Inicial é obrigatório.\n";
		}
		if(trim($('#prndtfim').val()) == ''){
			msg += "O campo Periodo de Execução Final é obrigatório.\n";
		}
		if($('#objdescricao').val() == ''){
			msg += "O campo Descrição é obrigatório.\n";
		}
		if($('#objbeneficio').val() == ''){
			msg += "O campo Benefícios Alcançados é obrigatório.\n";
		}
		if($('#objjustificativa').val() == ''){
			msg += "O campo Justificativa é obrigatório.\n";
		}
		if($('#objdata').val() == ''){
			msg += "O campo Data é obrigatório.\n";
		}
		
		if(msg){
			alert(msg);
			return false;
		}

		/*$('#pncnotacredito option').each(function(){
			$(this).attr('selected',true);
		});*/
		
		$('#formulario').submit();
	});

	/*if($('#prcnumero').val()){
		$('#prcnumero').val(mascaraglobal('#####.######/####-##',$('#prcnumero').val()));
	}*/

	/*$('#addNotacredito').click(function(){
		if($('#inputpncnotacredito').val()){
			$('#pncnotacredito').append("<option value='"+$('#inputpncnotacredito').val()+"' class='pncnotacreditooption'>"+$('#inputpncnotacredito').val()+"</option>");
			$('#inputpncnotacredito').val('');
		}
	});*/

	/*$('#removerNotacredito').click(function(){
		if($('#pncnotacredito option:selected').text() != ""){
			$('#pncnotacredito option:selected').remove();
		}
	});*/
	
	/*$('.pncnotacreditooption').live('dblclick', function(){
		$(this).remove();
	});*/

});

//
--></script>
<form action="" name="formulario" id="formulario" method="post">
<input type="hidden" name="acao" id="acao" value="1" />
<input type="hidden" name="objid" id="objid" value="<?php echo $obProcessoObjeto->objid; ?>" />
<input type="hidden" name="prcid" id="prcid" value="<?php echo $obProcesso->prcid; ?>" />
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td></td><td></td>
			<td rowspan="6">
				<?php 
				$tmcid = $_SESSION['academico']['tmcid'];
				if( $tmcid )
				{
					$docid = tcPegarDocid($tmcid);
					
					if($docid)
					{ 
						wf_desenhaBarraNavegacao( $docid , array( 'tmcid' => $tmcid ) );
					}				
				}
				?>
			</td>
		</tr>
		<!--  <tr>
			<td class="SubTituloDireita" valign="bottom">N° do processo de concessão:</td>
			<td>
				<?php 
					$prcnumero = $obProcesso->prcnumero;
					echo campo_texto( 'prcnumero', 'N', $habilitado, '', 30, 200, '#####.######/####-##', '','','','','id="prcnumero"' ); 
				?>
			</td>
		</tr> -->
		<tr>
			<td class="SubTituloDireita" valign="bottom">Executor N°:</td>
			<td>
				<?php 
					if($_SESSION['academico']['boPerfilSuperUsuario']){
						$sql = "select unicod as codigo, unicod || ' - ' || unidsc as descricao from public.unidade where orgcod = '".ORGAO."' and unistatus = 'A' order by unicod";
					} elseif(count($arUO)){
						$sql = "select unicod as codigo, unicod || ' - ' || unidsc as descricao from public.unidade where orgcod = '".ORGAO."' and unistatus = 'A' and unicod in ('". implode("','", $arUO)."') order by unicod ";
					} else {
						$sql = array();
					}
					
					$unicodexecutor = $obProcesso->unicodexecutor;
					$db->monta_combo( "unicodexecutor", $sql, $habilitado, 'Selecione', '', '', '', '300' );
				?>
			</td>
			
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Periodo de Execução:</td>
			<td>
				<?php 
					$executor = "<strong>CEFET-PA</strong>"; 
					$prndtinicio = $obProcesso->prndtinicio;
					echo campo_texto( 'prndtinicio', 'N', $habilitado, '', 10, 7, '##/####', '','','','','id="prndtinicio"');
					echo "&nbsp;&nbsp;&nbsp;";
					$prndtfim = $obProcesso->prndtfim;
					echo campo_texto( 'prndtfim', 'N', $habilitado, '', 10, 7, '##/####', '','','','','id="prndtfim"');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle">Descrição:</td>
			<td>
				<?php 
					$objdescricao = $obProcessoObjeto->objdescricao;
					echo campo_textarea( 'objdescricao', 'S', $habilitado, '', 60, 4, 250 );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle">Benefícios Alcançados:</td>
			<td>
				<?php 
					$objbeneficio = $obProcessoObjeto->objbeneficio;
					echo campo_textarea( 'objbeneficio', 'S', $habilitado, '', 60, 4, 250 );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle">Justificativas das atividades técnico pedagógicas:</td>
			<td>
				<?php 
					$objjustificativa = $obProcessoObjeto->objjustificativa;
					echo campo_textarea( 'objjustificativa', 'S', $habilitado, '', 60, 4, 250 );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle">Plano de Trabalho das Atividades Pedagógicas:</td>
			<td>
				<?php 
					$objplanotrabalho = $obProcessoObjeto->objplanotrabalho;
					echo campo_textarea( 'objplanotrabalho', 'N', $habilitado, '', 60, 4, 250 );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle">Resultados e impactos avançados em relação ao TAM (Termo de Acordo de Metas):</td>
			<td>
				<?php 
					$objtam = $obProcessoObjeto->objtam;
					echo campo_textarea( 'objtam', 'N', $habilitado, '', 60, 4, 250 );
				?>
			</td>
		</tr>
		<!--  <tr>
			<td width="200px" class="SubTituloDireita" valign="middle">Nota Crédito:</td>
			<td>
				<?php echo campo_texto( 'inputpncnotacredito', 'N', $habilitado, '', 22, 12, '', '','','','','id="inputpncnotacredito"' );?> <span id="addNotacredito" style="cursor: pointer"><img align="absmiddle" src="../imagens/seta_baixo.gif" /></span> <span id="removerNotacredito" style="cursor: pointer"><img align="absmiddle" src="../imagens/exclui_p.gif" /></span> <br />
				<select id="pncnotacredito" name="pncnotacredito[]" multiple="multiple" class="combo campoEstilo" style="width:153px;" size="5">
					<?php $arNotaCredito = $db->carregar("select pncnotacredito from academico.processonotacredito where prcid = {$obProcesso->prcid}"); ?>
					<?php $arNotaCredito = ($arNotaCredito) ? $arNotaCredito : array(); ?>
					<?php foreach($arNotaCredito as $notaCredito) :?>
						<option class="pncnotacreditooption" value="<?php echo $notaCredito['pncnotacredito']; ?>" ><?php echo $notaCredito['pncnotacredito']; ?></option>
					<?php endforeach;?>
				</select>
			</td>
		</tr>-->
		<tr>
			<td width="200px" class="SubTituloDireita" valign="middle">Número da Nota Crédito:</td>
			<td>
			<?php
				$tmcnc = $obTermoCooperacao->tmcnc;
				echo campo_texto( 'tmcnc', 'N', 'N', '', 20, 12, '###########', '', '', '','', "id='nc'" );
			?></td>
		</tr>
		<tr>
			<td width="200px" class="SubTituloDireita" valign="bottom">Portaria:</td>
			<td>
				<?php 
					/*$sql = "select prtid as codigo, prtdsc as descricao from academico.portaria where prtstatus = 'A'";
					$prtid = $obProcesso->prtid;
					$db->monta_combo( "prtid", $sql, $habilitado, 'Selecione', '', '' );*/
					$prtdsc = $db->pegaUm("select prtdsc from academico.portaria where prtstatus = 'A' and prtid = ".$obProcesso->prtid);
					echo $prtdsc;
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">UG/Gestão:</td>
			<td>
				<?php 
					if($_SESSION['academico']['boPerfilSuperUsuario']){
						$sql = "select ungcod as codigo, ungcod || ' - ' || ungdsc as descricao from academico.uougresponsavel where uurstatus = 'A' order by ungcod";
					} elseif(count($arUG)){
						$sql = "select ungcod as codigo, ungcod || ' - ' || ungdsc as descricao from academico.uougresponsavel where uurstatus = 'A' and unicod in ('". implode("','", $arUG)."') order by ungcod ";
					} else {
						$sql = array();
					}
					
					$ungcodgestor = $obProcesso->ungcodgestor;
					$db->monta_combo( "ungcodgestor", $sql, $habilitado, 'Selecione', '', '', '', '300' );
				?>
			</td>
		</tr>
		<tr>
			<td colspan="4"><strong>Desta forma, declaro que o objetivo proposto em referência foi fielmente cumprido.</strong></td>
		</tr>
		<tr>
			<!-- td class="SubTituloDireita" valign="middle">Local / UF:</td>
			<td>
				<?php 
					//echo "<strong>Belém</strong>";
				?>
			</td  -->
			<td class="SubTituloDireita" valign="bottom">Data:</td>
			<td>
				<?php 
					if($obProcessoObjeto->objdata){
						$objdata = $obData->formataData($obProcessoObjeto->objdata,"dd/mm/YYYY");
					} else {
						$objdata = date("d/m/Y");
					}
					echo campo_data2('objdata', 'S', $habilitado, 'Data', '##/##/####');
				?>
			</td>
		</tr>
		<!-- tr>
			<td colspan="4">Unidade Executor: <strong>CEFET-PA</strong><br /> <center><strong>Eliezer Moura Tavares / Diretor Administrativo e Planejamento</strong></center></td>
		</tr -->
		<tr>
			<td class="SubTituloDireita" colspan="4" align="center" style="text-align:center">
				<?php if($habilitado == 'S'): ?> 
         			<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" />
         		<?php endif;?>
         		<input type="button" value="Lista" onclick="window.location.href='academico.php?modulo=principal/listaProcesso&acao=A';" />
         	</td>
     	</tr>
	</tbody>
</table>
</form>