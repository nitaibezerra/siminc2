<?php

switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		$param 					 = array();
		$param['obrigatorio'] 	 = 'S';
		carregaEmpresaAndListaObra( $param );
		die;
}

if($_POST['requisicao']){
	$tecnico = new Tecnico_Empresa();
	$tecnico->$_POST['requisicao']();
}
if($_GET['temid']){
	$tecnico = new Tecnico_Empresa($_GET['temid']);
	extract($tecnico->recuperaDados());	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( ($_GET['temid'] ? "Editar" : "Cadastrar")." Técnico da Empresa", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

//Se o perfil não for super usuário, apenas as empresas ligadas ao usuário pela responsabilidade podem ser visualizadas.
//Se houver apenas uma empresa, não é necessário visualizar a combo para escolher a empresa.
if( !possui_perfil(array(PFLCOD_SUPER_USUARIO)) ){
	$usuarioResp = new UsuarioResponsabilidade();
	$arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida( $_SESSION['usucpf'] );
	if ( count($arEntidEmpresa) ){
		$grupoEmpresa  = new Supervisao_Grupo_Empresa();
		$param = array('sgrid' => $sgrid,
					   'entid' => $arEntidEmpresa);
		foreach($grupoEmpresa->pegaSgeid( $param ) as $grupo){
			$arrGrupo[] = $grupo;
		}
	}
	$arrEmpresa = $usuarioResp->pegaSgeidEmpresaPermitida( $_SESSION['usucpf'] );
}
if($db->testa_superuser()){
	$perfil_super_user = true;
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>
	.hidden{display:none}
	.link{cursor:pointer}
</style>
<form id="formulario" name="formulario" method="post" action="" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="temid" id="temid" value="<?php echo $temid ?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<?php if($perfil_super_user || count($arrGrupo) > 1):  ?>
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
	<?php else: ?>
		<tr class="hidden">
			<td>
				<input type="hidden" name="sgrid" id="sgrid" value="<?php echo $arrGrupo[0] ?>" />
			</td>
		</tr>
	<?php endif; ?>
	<?php if($perfil_super_user || count($arrEmpresa) > 1):  ?>
		<tr>
	        <td class="SubTituloDireita">Empresa</td>
			<td id="tdComboEmpresa">
				<?php
				if(count($arrGrupo) == 1){
					$sgrid = $sgrid ? $sgrid : $arrGrupo[0];
				}else{
					$sgrid = $sgrid ? $sgrid : null;
				}
				if ( !empty($sgrid) ){
	                $grupoEmpresa = new Supervisao_Grupo_Empresa();
	                $dados = $grupoEmpresa->listaCombo( array('sgrid' => $sgrid) );
	            	$db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'sgeid');
				}else{
					echo "Selecione o grupo.";	
				}
	            ?>
			</td>
		</tr>
	<?php else: ?>
		<tr class="hidden">
			<td>
				<input type="hidden" name="sgeid" id="sgeid" value="<?php echo $arrEmpresa[0] ?>" />
			</td>
		</tr>
	<?php endif; ?>
	<tr>
		<td class="SubTituloDireita">CPF</td>
		<td>
			<?php echo campo_texto('temcpf', 'S', "S", '', 20, 14, '[###.###.###-##]', '', '', '', 0, 'id="temcpf"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome</td>
		<td>
			<?php echo campo_texto('temnome', 'S', "S", '', 60, 255, '', '', '', '', 0, 'id="temnome"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo</td>
		<td>
			<?php echo campo_texto('temcargo', 'S', "S", '', 60, 255, '', '', '', '', 0, 'id="temcargo"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nº CREA/CAU</td>
		<td>
			<?php echo campo_texto('temnumcreacau', 'S', "S", '', 40, 255, '', '', '', '', 0, 'id="temnumcreacau"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Capacitação</td>
		<td>
			<?php echo campo_data2( 'temdtcapacitacao', 'N', 'S', '', 'S','','' ); ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" >Status</td>
		<td>
			<?php
                $arrStatus[0] = array("codigo" => "A", "descricao" => "Ativo");
                $arrStatus[1] = array("codigo" => "I", "descricao" => "Inativo");
            	$db->monta_combo("temstatus", $arrStatus, 'S', "Selecione...", "", '', '', '', 'S', 'temstatus');
            ?>
		</td>
	</tr>
	<tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
			<input type="button" value="Salvar"      name="btn_salvar"   onclick="salvarTecnico();">
            <input type="button" value="Cancelar"    name="btn_cancelar" onclick="window.location='obras2.php?modulo=principal/tecnicoEmpresa&acao=A'">
        </td>
    </tr>
</table>
</form>
<script type="text/javascript">
function carregaDependenciaEmpresa( sgeid ){
	
}

function carregaDependenciaGrupo( sgrid ){
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadTecnicoEmpresa&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(listaObras)":true}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').html( comboEmpresa );
			
//			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
//			$('#divListaObra').html( listaObra );
			
		});
	}else{
		$('#tdComboEmpresa').html('Selecione o grupo.');
	}
}

function salvarTecnico()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if($("#temcpf").val() && !validar_cpf($("#temcpf").val()))
	{
		erro = 1;
		alert('CPF inválido!');
		return false;
	}
	if(erro == 0){
		$('#requisicao').val( 'salvarTecnico' );
    	$('#formulario').submit();
	}
}
</script>