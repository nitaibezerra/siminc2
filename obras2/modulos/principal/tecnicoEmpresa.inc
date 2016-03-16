<?php
switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		$param 					 = array();
		$param['obrigatorio'] 	 = 'N';
		carregaEmpresaAndListaObra( $param );
		die;
}

if($_POST['requisicao'] == "pesquisarTecnico"){
	$_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];
	$_POST['sgeid'] = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
	$_POST['entid'] = $_POST['entid_disable'] ? $_POST['entid_disable'] : null;
	extract($_POST);
	$arrWhere = $_POST;
}

if($_POST['requisicao'] == "excluirTecnico"){
	$tecnico = new Tecnico_Empresa($_POST['temid']);
	$tecnico->excluirTecnico($_POST['temid']);
	$tecnico->commit();
	
	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/tecnicoEmpresa&acao=A\';
		 </script>');
}

if($_POST['requisicao'] == "ativacaoTecnico"){
	$tecnico = new Tecnico_Empresa($_POST['temid']);
	$tecnico->ativacaoTecnico( $_POST['temid'], $_POST['ativacao'] );
	$tecnico->commit();
	
	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/tecnicoEmpresa&acao=A\';
		 </script>');
}

if($_GET['requisicao'] == "exibirHistoricoTecnico"){
	monta_titulo( 'Histórico de Alterações', '' );
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script>
		function exibirDadosTecnico(hteid)
		{
			$.ajax({
			   type: "POST",
			   url: "obras2.php?modulo=principal/tecnicoEmpresa&acao=A",
			   data: "requisicaoAjax=exibirHistoricoTecnico&classe=Historico_Tecnico_Empresa&hteid="+hteid,
			   success: function(msg){
			   		$('#div_dados_tecnico').html( msg );
			   }
			 });
		}
	</script>
	<?php
	$historico_tecnico = new Historico_Tecnico_Empresa();
	$historico_tecnico->listaHistoricoTecnico($_GET['temid']);
	echo "<div id=\"div_dados_tecnico\" ></div>";
	echo "<center><input type='button' name='tbn_fechar' value='Fechar' onclick='window.close()' /></center>";
	exit;
}
if($_REQUEST['requisicaoAjax']){
	$n = new $_REQUEST['classe']();
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}
		
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( 'Técnicos da Empresa', '' );

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
<form id="formulario_pesquisa" name="formulario_pesquisa" method="post" action="" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="ativacao" id="ativacao" value="" />
<input type="hidden" name="temid" id="temid" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<?php if($perfil_super_user || count($arrGrupo) > 1):  ?>
		<tr>
	        <td class="SubTituloDireita" width="35%">Grupo</td>
			<td>
				<?php
	                $grupo = new Supervisao_Grupo();
	                $dados = $grupo->listaCombo();
	                
	            	$db->monta_combo("sgrid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupo", '', '', '', 'N', 'sgrid');
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
                    $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                    $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                    $db->monta_combo("entid", $empresas, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'entid');
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
			<?php echo campo_texto('temcpf', 'N', "S", '', 20, 14, '[###.###.###-##]', '', '', '', 0, 'id="temcpf"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome</td>
		<td>
			<?php echo campo_texto('temnome', 'N', "S", '', 60, 255, '', '', '', '', 0, 'id="temnome"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo</td>
		<td>
			<?php echo campo_texto('temcargo', 'N', "S", '', 60, 255, '', '', '', '', 0, 'id="temcargo"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nº CREA/CAU</td>
		<td>
			<?php echo campo_texto('temnumcreacau', 'N', "S", '', 40, 255, '', '', '', '', 0, 'id="temnumcreacau"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Capacitação</td>
		<td>
			<?php $temdtcapacitacao = formata_data_sql($_POST['temdtcapacitacao']) ?>
			<?php echo campo_data2( 'temdtcapacitacao', 'N', 'S', '', 'S','','' ); ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" >Status</td>
		<td>
			<label for="temvigente_ativo">
				<input type="radio" name="temvigente" id="temvigente_ativo" value="A" <?php echo ($temvigente == 'A' || empty($temvigente) ? "checked='checked'" : '') ?>>
				Ativo
			</label>
			<label for="temvigente_inativo">
				<input type="radio" name="temvigente" id="temvigente_inativo" value="I" <?php echo ($temvigente == 'I' ? "checked='checked'" : '') ?>>
				Inativo
			</label>
			<?php
//                $arrStatus[0] = array("codigo" => "A", "descricao" => "Ativo");
//                $arrStatus[1] = array("codigo" => "I", "descricao" => "Inativo");
//            	$db->monta_combo("temvigente", $arrStatus, 'S', "Selecione...", "", '', '', '', '', 'temvigente');
            ?>
		</td>
	</tr>
	<tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
			<input type="button" value="Pesquisar" name="btn_pesquisar" onclick="pesquisarTecnico();">
            <input type="button" value="Limpar"    name="btn_limpar"    onclick="limparTecnico()">
        </td>
    </tr>
    <tr>
    	<td colspan="2" ><span title="Cadastrar Novo Técnico" class="link" style="font-weight:bold" onclick="location.href='?modulo=principal/cadTecnicoEmpresa&acao=A'"><img style="vertical-align:middle" src="../imagens/gif_inclui.gif" /> Novo Técnico</span></td>
	</tr>
</table>
</form>
<?php
$tecnico  = new Tecnico_Empresa();
$arrWhere = $arrWhere ? $arrWhere : array();
$sql 	  = $tecnico->listaSql($arrWhere);

$arrCabecalho = array("Ação","Grupo","Empresa","CPF","Nome","Cargo","Nº CREA/CAU","Data de Cadastro","Data de Capacitação","Status");
$db->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");
?>
<script type="text/javascript">
$(function() {
	<?php if($_SESSION['obras2']['tecnico_empresa']['alert']): ?>
		alert('<?php echo $_SESSION['obras2']['tecnico_empresa']['alert'] ?>');
		<?php unset($_SESSION['obras2']['tecnico_empresa']['alert']) ?>
	<?php endif; ?>
});

function editarTecnico( temid ){
	location.href = 'obras2.php?modulo=principal/cadTecnicoEmpresa&acao=A&temid=' + temid;
}

function excluirTecnico( temid ){
    if(confirm("Deseja realmente excluir definitivamente este Técnico?")){
    	$('#temid').val( temid );
    	$('#requisicao').val( 'excluirTecnico' );
    	$('#formulario_pesquisa').submit();
    }
}

function inativarTecnico( temid, ativacao ){
	var txtMsg = (ativacao == 'A' ? 'ativar' : 'desativar');
    if(confirm("Deseja realmente " + txtMsg + " este Técnico?")){
    	$('#temid').val( temid );
    	$('#requisicao').val( 'ativacaoTecnico' );
    	$('#ativacao').val( ativacao );
    	$('#formulario_pesquisa').submit();
    }
}

function carregaDependenciaEmpresa( sgeid ){
	
}

function carregaDependenciaGrupo( sgrid ){
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/tecnicoEmpresa&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(listaObras)":true}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').empty().html( comboEmpresa );
			
//			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
//			$('#divListaObra').html( listaObra );
			
		});
	}else{
		$('#tdComboEmpresa').html('Selecione o grupo.');
	}
}

function pesquisarTecnico()
{
	$('#requisicao').val( 'pesquisarTecnico' );
    $('#formulario_pesquisa').submit();
}

function limparTecnico()
{
	window.location.href=window.location;
}

function historicoTecnico(temid)
{
	window.open('?modulo=principal/tecnicoEmpresa&acao=A&requisicao=exibirHistoricoTecnico&temid='+temid, 
			'TecnicoEmpresa',
			"height=600,width=800,scrollbars=yes,top=50,left=200" ).focus();
}
</script>