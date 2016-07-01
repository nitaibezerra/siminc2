<?php
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}
switch ( $_POST['op'] ){
	case 'apagar':
		$os = new Supervisao_Os( $_POST['sosid'] );
		$os->sosstatus = 'I';
		$os->salvar();
		
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/listOs&acao=A';
			 </script>");
		break;
	case 'pesquisar':
		$_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];
		$_POST['sgeid'] = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
		extract($_POST);
		$arrWhere = $_POST;
		break;
	case 'exportar':
		$_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];
		$_POST['sgeid'] = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
		extract($_POST);
		$arrWhere = $_POST;
		$os = new Supervisao_Os();
		$arrWhere = $arrWhere ? $arrWhere : array();
        $os->listaOS($arrWhere, true);
		break;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
criaAbaOS();
monta_titulo( 'Lista de OS', '' );
?>
<style>
	.link{cursor:pointer}
</style>


<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>


<form name="formulario_acao" id="formulario_acao" method="post" action="">
    <input type="hidden" name="sosid" id="sosid" value=""/>
    <input type="hidden" name="op" id="op" value=""/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
	        <td class="SubTituloDireita" width="35%">Grupo</td>
			<td>
				<?php
	                $grupo = new Supervisao_Grupo();
	                $dados = $grupo->listaCombo();
	                
	            	$db->monta_combo("sgrid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupo", '', '', '', 'S', 'sgrid');
	            ?>
			</td>
		</tr>
		<tr>
	        <td class="SubTituloDireita">Empresa</td>
			<td id="tdComboEmpresa">
				<?php
//				if ( !empty($sgrid) ){
//	                $grupoEmpresa = new Supervisao_Grupo_Empresa();
//	                $dados = $grupoEmpresa->listaCombo( array('sgrid' => $sgrid) );
//	                
//	            	$db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaEmpresa", '', '', '', 'S', 'sgeid');
//				}else{
//					echo "Selecione o grupo.";	
//				}
	            ?>
                <?php
                $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                echo $db->monta_combo("entid", $empresas, 'S','Selecione...','carregaDependenciaEmpresa', '', '',200,'N','entid'); 
                ?>
			</td>
		</tr>
		<tr>
	        <td class="SubTituloDireita">Nota de Empenho</td>
			<td id="tdEmpenho">
				<?php
				if ( !empty($semid) ){
	                $empenho = new Supervisao_Empenho();
	                $dados = $empenho->listaComboPorEmpresa( array('sgrid' => $_POST['sgrid'], 'sgeid'=> $_POST['sgeid']) );
	                
	            	$db->monta_combo("semid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'semid');
				}else{
					echo "Selecione a empresa.";	
				}
	            ?>
			</td>
		</tr>
	    <tr>
	    	<td class="SubTituloDireita">OS Nº</td>
	        <td>
				<?=campo_texto('sosnum', 'N', "S", '', 10, 10, '[#]', '', 'right', '', 0, 'id="sosnum"', ''); ?>
	        </td>
		</tr>
		<tr>
	    	<td class="SubTituloDireita">Situação da OS</td>
	        <td>
				<?php criaComboWorkflow(TPDID_OS) ?>
	        </td>
		</tr>
		<?php
					montaComboTerritorio("listobra",array("estuf"=>array("obrigatorio"=>"N","value"=>$_POST['listobraestuf']),"mescod"=>array("obrigatorio"=>"N","disabled"=>"S","value"=>$_POST['listobramescod']),"miccod"=>array("disabled"=>"S","value"=>$_POST['listobramiccod']),"muncod"=>array("disabled"=>"S","value"=>$_POST['listobramuncod'])));
		?>
		<tr>
	    	<td class="SubTituloDireita">Possui supervisão atrasada?</td>
	        <td>
				<input type='radio' name='rdb_atrasada' <?php echo $_POST['rdb_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim' />Sim
				<input type='radio' name='rdb_atrasada' <?php echo $_POST['rdb_atrasada'] == "nao" ? "checked='checked'" : "" ?> value='nao' />Não
				<input type='radio' name='rdb_atrasada' <?php echo $_POST['rdb_atrasada'] == "todas" ? "checked='checked'" : "" ?> value='todas' />Todas
	        </td>
		</tr>

        <tr>
            <td class="SubTituloDireita">Supervisão homologada com atraso</td>
            <td>
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim' />Sim
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "nao" ? "checked='checked'" : "" ?> value='nao' />Não
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "todas" ? "checked='checked'" : "" ?> value='todas' />Todas
            </td>
        </tr>

		<tr>
	    	<td class="SubTituloDireita">Emergencial</td>
	        <td>
				<input type="radio" <?php echo ($sosemergencial == 't'     ? 'checked="checked"' : ''); ?>  name="sosemergencial" value="t" />Sim
				<input type="radio" <?php echo ($sosemergencial == 'f'     ? 'checked="checked"' : ''); ?>  name="sosemergencial" value="f" />Não
				<input type="radio" <?php echo ($sosemergencial == 'todas' ? 'checked="checked"' : ''); ?> name="sosemergencial" value="todas" />Todas
	        </td>
		</tr>
        <tr>
	    	<td class="SubTituloDireita">OS prorrogada</td>
	        <td>
				<input type="radio" <?php echo ($sosprorrogada == 't'     ? 'checked="checked"' : ''); ?>  name="sosprorrogada" value="t" />Sim
				<input type="radio" <?php echo ($sosprorrogada == 'f'     ? 'checked="checked"' : ''); ?>  name="sosprorrogada" value="f" />Não
				<input type="radio" <?php echo ($sosprorrogada == 'todas' ? 'checked="checked"' : ''); ?> name="sosprorrogada" value="todas" />Todas
	        </td>
		</tr>
                <tr>
    <td class="subtitulodireita" width="190px">Início da Execução:</td>
    <td>
        de: <input type="text" id="inicio_exec_de" name="inicio_exec_de" value="<?php echo $inicio_exec_de;?>" size="15" maxlength="10" class="normal calendar" >
        &nbsp;
        até: <input type="text" id="inicio_exec_ate" name="inicio_exec_ate" value="<?php echo $inicio_exec_ate;?>" size="15" maxlength="10" class="normal calendar">
    </td>
</tr>
<tr>
    <td class="subtitulodireita" width="190px">Término da Execução:</td>
    <td>
        de: <input type="text" id="termino_exec_de" name="termino_exec_de" value="<?php echo $termino_exec_de;?>" size="15" maxlength="10" class="normal calendar" >
        &nbsp;
        até: <input type="text" id="termino_exec_ate" name="termino_exec_ate" value="<?php echo $termino_exec_ate;?>" size="15" maxlength="10" class="normal calendar">
    </td>
</tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data de Emissão:</td>
            <td>
                de: <input type="text" id="dt_emissao_de" name="dt_emissao_de" value="<?php echo $dt_emissao_de;?>" size="15" maxlength="10" class="normal calendar" >
                &nbsp;
                até: <input type="text" id="dt_emissao_ate" name="dt_emissao_ate" value="<?php echo $dt_emissao_ate;?>" size="15" maxlength="10" class="normal calendar">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top">Tipo de Supervisão</td>
            <td colspan="3">
                <input type="radio" name="sosterreno" id="" value="f" <?= ($sosterreno == 'f') ? 'checked="checked"' : '' ?> > Obra
                <input type="radio" name="sosterreno" id="" value="t"  <?= ($sosterreno == 't') ? 'checked="checked"' : '' ?> > Implantação de Obra
                <input type="radio" name="sosterreno" id="" value=""  <?= (empty($sosterreno)) ? 'checked="checked"' : '' ?> > Todas
            </td>
        </tr>

		<tr>
	        <td bgcolor="#c0c0c0" colspan="2" align="center">
				<input type="button" value="Pesquisar" name="btn_pesquisar" onclick="pesquisarOS();">
	            <input type="button" value="Limpar"    name="btn_limpar"    onclick="limparOS()">
	            <input type="button" value="Exportar"  name="btn_exportar"    onclick="exportarExcel()">
	        </td>
	    </tr>
	    <tr>
<?php
	if ( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC) ) ):
?>	    
	    	<td colspan="2" >
	    		<span title="Cadastrar Nova Orderm de Serviço" class="link" style="font-weight:bold" onclick="location.href='?modulo=principal/cadOs&acao=A'">
	    			<img style="vertical-align:middle" src="../imagens/gif_inclui.gif" /> Nova Orderm de Serviço
	    		</span>
	    	</td>
<?php 
	endif;
?>	    	
		</tr>
	</table>
</form>
<?php
$os = new Supervisao_Os();
$arrWhere = $arrWhere ? $arrWhere : array();

$os->listaOS($arrWhere);
?>
<script type="text/javascript">
function imprimirOs( sosid ){
	return windowOpen( '?modulo=principal/popupImpressaoOS&acao=A&sosid=' + sosid,'blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
				
function editarOs( sosid ){
	location.href = '?modulo=principal/cadOs&acao=E&sosid=' + sosid;
}

function excluirOs( sosid ){
    if(confirm("Deseja realmente excluir essa OS?")){
    	$('#sosid').val( sosid );
    	$('#op').val( 'apagar' );
    	$('#formulario_acao').submit();
    }
}
//function carregaDependenciaEmpresa( sgeid ){
//	var sgrid = $('#sgrid').val();
//	if ( sgeid ){
//		$.post("?modulo=principal/cadOs&acao=A", {"sgeid":sgeid, "ajax":"carregaEmpenho"}, function(data){
//			$('#tdEmpenho').html( data );
//		});
//		
//		var orgid = $('#orgid').val();
//		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "sgeid":sgeid, "ajax":"carregaEmpresaAndListaObra", "not(comboGrupoEmpresa)":true}, function(data){
//			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
//			$('#divListaObra').html( listaObra );
//		});
//	}else{
//		$('#tdEmpenho').html('Selecione a empresa.');
//		if ( sgrid ){
//			carregaDependenciaGrupo( sgrid );
//		}
//	}
//}
function carregaDependenciaEmpresa( entid ){
	var sgrid = $('#sgrid').val();
	if ( entid ){
		$.post("?modulo=principal/cadOs&acao=A", {"entid":entid,'sgrid':sgrid, "ajax":"carregaEmpenhoPorEmpresa"}, function(data){
			$('#tdEmpenho').html( data );
		});
		
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "sgeid":sgeid, "ajax":"carregaEmpresaAndListaObra", "not(comboGrupoEmpresa)":true}, function(data){
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
		});
	}else{
		$('#tdEmpenho').html('Selecione a empresa.');
		if ( sgrid ){
			carregaDependenciaGrupo( sgrid );
		}
	}
}

function carregaDependenciaGrupo( sgrid ){
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid}, function(data){
//			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
//			$('#tdComboEmpresa').html( comboEmpresa );
			
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
			
		});
	}else{
		$('#tdComboEmpresa').html('Selecione o grupo.');
	}
}

function pesquisarOS()
{
	$('#op').val( 'pesquisar' );
    $('#formulario_acao').submit();
}

function limparOS()
{
	window.location.href=window.location;
}

function exportarExcel()
{
	$('#op').val( 'exportar' );
    $('#formulario_acao').submit();
}

function abrePopupProrrogacao( sosid ){
	popupProrrogacao = window.open( 'obras2.php?modulo=principal/cadOS_prorrogacao&acao=E&sosid='+sosid, 'Prorrogacao', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
}
</script>
<script type="text/javascript">

    $(function(){

        jQuery(function($){
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });


        jQuery('.calendar').mask('99/99/9999');
        jQuery('.calendar').datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim:'drop'
        });
    });
</script>
