<?php

cte_verificaSessao();

include_once( APPRAIZ. "includes/classes/AbaAjax.class.inc" );
include_once( APPRAIZ. "cte/classes/MonitoramentoInterno.class.inc" );

//ver( 123, $_SESSION['inuid'], $_POST['sbaid'] );

$obMonitoramentoInterno = new MonitoramentoInterno();

if(isset($_POST['stMetodo'])) {
	$obMonitoramentoInterno->carregarPorInuidPpsidAno( $_SESSION['inuid'], $_REQUEST['ppsid'], $_POST['mniano'] );
	$obMonitoramentoInterno->$_POST['stMetodo']( $_POST['sbaid']);
	die();
}

if( $_REQUEST["formulario"] ){
	
	$obMonitoramentoInterno->carregarPorInuidPpsidAno( $_SESSION['inuid'], $_REQUEST['ppsid'], $_POST['mniano'] );
	
	$arDados = array( 'inuid', 'ppsid', 'mniqtd', 'mniano' );
	$obMonitoramentoInterno->popularObjeto( $arDados );
	
	$obMonitoramentoInterno->salvar();
	$obMonitoramentoInterno->commit();
    $obMonitoramentoInterno->sucesso( $modulo, '&ppsid='.$obMonitoramentoInterno->ppsid.'&mniano='.$obMonitoramentoInterno->mniano );
	
}

$sql = "select  s.sbaid, s.undid, s.prgid, s.sbaordem, s.ppsid, s.frmid, s.sbadsc, sbastgmpl,
				d.dimcod, d.dimdsc, ad.ardcod, arddsc, i.indcod, i.inddsc, f.frmdsc, u.undid, u.unddsc,
				case 
					when ( s.sbaporescola = true ) then 'Por Escola'
					else 'Global'
				end as sbaporescola
			from cte.subacaoindicador s
				inner join cte.formaexecucao f on f.frmid = s.frmid
				inner join cte.unidademedida u on u.undid = s.undid
				inner join cte.acaoindicador a on a.aciid = s.aciid
				inner join cte.pontuacao p on p.ptoid = a.ptoid
				inner join cte.criterio c on c.crtid = p.crtid
				inner join cte.indicador i on i.indid = c.indid
				inner join cte.areadimensao ad on ad.ardid = i.ardid
				inner join cte.dimensao d on d.dimid = ad.dimid
		where p.inuid = {$_SESSION['inuid']}
		and s.ppsid = {$_REQUEST["ppsid"]}
		and ptostatus = 'A'";

$arDados = $db->pegaLinha( $sql );

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

cte_montaTitulo( $titulo_modulo, '&nbsp;', '?modulo=principal/monitoramentoInterno/lista&acao=A&mniano='.$_REQUEST['mniano'].'&ppsid='.$_REQUEST['ppsid'].'' );

if( $arDados['undid'] == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_MUN ){
	$stNomeMetodo = 'montarFormularioEscolasPorAno';
}
else{
	$arUnidades = cte_recuperarArUnidadesExigemCPF();
	$stNomeMetodo 	= in_array( $arDados['undid'], $arUnidades ) ? 'montarFormularioGlobalComCPFPorAno' : 'montarFormularioGlobalSemCPFPorAno';
}

//CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_MUN

$sql = "SELECT

			case 
				when ( indqtdporescola = true ) then 'Por Escola'
				else 'Global'
			end as indqtdporescola			
		FROM cte.proposicaosubacao 
		WHERE ppsid = '".$arDados['ppsid']."'";
$arCronoPPSID = $db->pegaLinha($sql);

?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<colgroup>
		<col width="25%" />
		<col width="10%" />
		<col width="10%" />
		<col width="55%" />
	</colgroup>

	<tbody>
	  
		<tr>
			<td class="SubTituloDireita">Dimensão:</td>
			<td colspan="3"><?php echo $arDados['dimcod'].' - '.$arDados['dimdsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Área:</td>
			<td colspan="3"><?php echo $arDados['ardcod'].' - '.$arDados['arddsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Indicador:</td>
			<td colspan="3"><?php echo $arDados['indcod'].' - '.$arDados['inddsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descricção da Subação:</td>
			<td colspan="3"><?php echo $arDados['dimcod'].'.'.$arDados['ardcod'].'.'.$arDados['indcod'].' - '.$arDados['sbaordem'].' '.$arDados['sbadsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Estratégia da Implementação:</td>
			<td colspan="3"><?php echo $arDados['sbastgmpl'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade de Medida:</td>
			<td colspan="3"><?php echo $arDados['unddsc'] ?></td>
		</tr>		
		<tr>
			<td class="SubTituloDireita">Forma de Execução:</td>
			<td colspan="3"><?php echo $arDados['frmdsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cronograma da Subação:</td>
			<td><?php echo $arDados['sbaporescola'] ?></td>
			<td class="SubTituloDireita">Cronograma do Guia:</td>
			<td><?php echo $arCronoPPSID['indqtdporescola'] ?></td>
		</tr>
	</tbody>
</table>
<?php
/**
 * Cria Aba Ajax
 */
$obAbaAjax = new AbaAjax(null,true, true, true, true);

$url = $_SERVER['REQUEST_URI'];
	  	      
$arAnos = array( 2008, 2009, 2010, 2011 );
$anoSelecionado = isset( $_REQUEST["mniano"] ) ? $_REQUEST["mniano"] : date( 'Y' );
foreach( $arAnos as $count => $ano ){

	$arAbas[$count]['descricao'] = $ano;
	if( $anoSelecionado == $ano ){
		$arAbas[$count]['padrao'] = true;
	}
	$arAbas[$count]['url'] = $url;
	$arAbas[$count]['parametro'] = "stMetodo=$stNomeMetodo&mniano=$ano&sbaid={$arDados['sbaid']}";
}

$obAbaAjax->criaAba($arAbas, 'divMonitoramentoInterno');

?>
<!-- 
<center><input type="button" value="voltar" onclick="javascript:history.go(-1);"></center>
<br><br><center><input type="button" value="voltar" onclick="voltarTelaPrincipal()"></center> 
-->



<div id="divMonitoramentoInterno"></div>

<script language="javascript" type="text/javascript">
	
	function validarCPF( obj ){
		
		if( obj.value ){
			if( !validar_cpf( obj.value  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				obj.value = "";	
				document.getElementById( "dunnome" ).value = "";
			}
		}
	}
	
	function procurarNome( cpf ){

		var comp = new dCPF();
		var nome = document.getElementById( 'dunnome' );
		var rg = document.getElementById( 'dunrg' );
		var orgaorg = document.getElementById( 'dunorgaorg' );
		var ufrg = document.getElementById( 'dunufrg' );
		
		comp.buscarDados( cpf );
		
		if( comp.dados.no_pessoa_rf != '' ){
			nome.value = comp.dados.no_pessoa_rf;
			rg.value = comp.dados.nu_rg;
			orgaorg.value = comp.dados.ds_orgao_expedidor_rg;
			nome.readOnly = true;
		}
	}	
	
	function validarFormulario( idFormulario ){
		
		if( !jQuery('#mniqtd').val() ){
			alert( 'A quantidade do programa é de preenchimento obrigatório' );
			return false;
		}
		
		
		document.getElementById( idFormulario ).submit();
	}
		
	function fechar(){
		window.location.href = '/cte/cte.php?modulo=principal/monitoramentoInterno/lista&acao=A';
	}
	
	function popupSelecionarEscolas( mniano, inuid, ppsid ){
	
        return windowOpen( '/cte/cte.php?modulo=principal/monitoramentoInterno/cadastrarEscolasMonitoramentoInterno&acao=A&mniano=' + mniano + '&inuid=' + inuid + '&ppsid=' + ppsid,
                           'popupSelecionarEscolas',
                           'height=400, width=800, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
        return false;
    }	
	
	function popupSelecionarCursista( mniano, inuid, ppsid ){
	
        return windowOpen( '/cte/cte.php?modulo=principal/monitoramentoInterno/cadastrarCursistasMonitoramentoInterno&acao=A&mniano=' + mniano + '&inuid=' + inuid + '&ppsid=' + ppsid,
                           'popupSelecionarEscolas',
                           'height=400, width=800, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
        return false;
    }
    function voltarTelaPrincipal(){
    	window.location.href = '<?='?modulo=principal/monitoramentoInterno/lista&acao=A&mniano='.$_REQUEST['mniano'].'&ppsid='.$_REQUEST['ppsid'].'';?>';
    	
    }	
	
</script>