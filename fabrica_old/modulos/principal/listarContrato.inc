<?php

include APPRAIZ."fabrica/classes/Contrato.class.inc";


if($_REQUEST['requisicao'] && $_REQUEST['ctrid']){
	$ctrid = $_REQUEST['ctrid'];
	$contrato = new Contrato($ctrid);
	$contrato->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=principal/listarContrato&acao=A");
	}
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Contratos", "link"=> "fabrica.php?modulo=principal/listarContrato&acao=A");
$menu[1] = array("descricao" => "Cadastrar Contrato", "link"=> "fabrica.php?modulo=principal/cadContrato&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=principal/listarContrato&acao=A");

$titulo = "Listar Contratos";
monta_titulo( $titulo, '&nbsp;' );

$contrato = new Contrato();

if($_REQUEST['requisicao'] && !$_REQUEST['ctrid']){
	$contrato->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
	function alterarContrato(ctrid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadContrato&acao=A&ctrid=' + ctrid;
	}
	
	function excluirContrato(ctrid)
	{
		if(confirm('Deseja realmente excluir o contrato?')){
			window.location.href = 'fabrica.php?modulo=principal/listarContrato&acao=A&requisicao=excluirContrato&reload=true&ctrid=' + ctrid;
		}
	}
	
	function novoContrato()
	{
		window.location.href = 'fabrica.php?modulo=principal/cadContrato&acao=A';
	}
	
	function pesquisarContrato()
	{
		document.getElementById('form_listar_contrato').submit();
	}
	
	function addEmpresa()
	{
		var entid = $('#entidcontratado').val();
		if(entid){
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&entid=' + entid + '&tipo=<?=TIPO_EMPRESA_FABRICA?>','Empresa','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&tipo=<?=TIPO_EMPRESA_FABRICA?>','Empresa','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
		}
		void(0);
	}
	
	function addGestor()
	{
		var entid = $('#entidgestor').val();
		if(entid){
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&funcao=gestor&tipo=<?=TIPO_GESTOR_CONTRATO?>&entid=' + entid,'Gestor','scrollbars=yes,height=450,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&funcao=gestor&tipo=<?=TIPO_GESTOR_CONTRATO?>','Gestor','scrollbars=yes,height=450,width=840,status=no,toolbar=no,menubar=no,location=no');
		}
		void(0);
	}
	
	function addCoGestor()
	{
		var entid = $('#entidcogestor').val();
		if(entid){
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&funcao=cogestor&tipo=<?=TIPO_GESTOR_CONTRATO?>&entid=' + entid,'Co-Gestor','scrollbars=yes,height=500,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&funcao=cogestor&tipo=<?=TIPO_GESTOR_CONTRATO?>','Co-Gestor','scrollbars=yes,height=500,width=840,status=no,toolbar=no,menubar=no,location=no');
		}
		void(0);
	}
	
	function limparEmpresa()
	{
		$('[name=entnome_empresa],#entidcontratado').val("");
	}
	
	function limparGestor()
	{
		$('[name=entnome_gestor_contrato],#entidgestor').val("");
	}
	
	function limparCoGestor()
	{
		$('[name=entnome_co_gestor_contrato],#entidcogestor').val("");
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_contrato" id="form_listar_contrato" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Número do Contrato:</td>
			<td>
				<input type="hidden" name="requisicao" value="pesquisarContrato" />
				<?=campo_texto("ctrnumero","N","S","Número do Contrato","40","120","","") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">CNPJ:</td>
			<td><? echo campo_texto('entnumcpfcnpj', 'N', 'S', 'CNPJ', 22, 19, "##.###.###/####-##", "", '', '', 0, 'id="entnumcpfcnpj"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Nome da Empresa:</td>
			<td><?=campo_texto("entnome", 'N', 'S', "CNPJ", "60", "254", "", "") ?></td>
		</tr>
		<!-- 
		<tr>
			<td class="SubtituloDireita" width="25%" >Nome da Empresa:</td>
			<td>
				<?php
				if($entidcontratado){ 
					require_once APPRAIZ . "includes/classes/entidades.class.inc";
					$entidade = new Entidades();
					$entidade->carregarPorEntid($entidcontratado);
					$entnome_empresa = $entidade->getEntnome();
				}
				?>
				<?=campo_texto("entnome_empresa","S","N","Empresa do Contrato","60","120","","") ?>
				<?php if(!$entidcontratado): ?>
					<img src="../imagens/gif_inclui.gif" id="img_add_empresa" class="middle link" title="Adicionar Empresa" onclick="addEmpresa()">
				<?php else: ?>
					<img src="../imagens/alterar.gif" id="img_add_empresa" class="middle link" title="Alterar Empresa" onclick="addEmpresa()">
				<?php endif; ?>
				<img src="../imagens/excluir.gif" class="middle link" title="Limpar Empresa" onclick="limparEmpresa()">
				<input type="hidden" name="entidcontratado" id="entidcontratado" value="<?=$entidcontratado?>" />
			</td>
		</tr>
		 -->

		<!--<tr>
			<td class="SubtituloDireita">Gestor do Contrato:</td>
			<td>
				<?php
				if($entidgestor){ 
					require_once APPRAIZ . "includes/classes/entidades.class.inc";
					$entidade = new Entidades();
					$entidade->carregarPorEntid($entidgestor);
					$entnome_gestor_contrato = $entidade->getEntnome();
				}
				?>
				<?=campo_texto("entnome_gestor_contrato","S","N","Gestor do Contrato","60","120","","") ?>
				<?php if(!$entidgestor): ?>
					<img src="../imagens/gif_inclui.gif" id="img_add_gestor" class="middle link" title="Adicionar Gestor" onclick="addGestor()">
				<?php else: ?>
					<img src="../imagens/alterar.gif" id="img_add_gestor" class="middle link" title="Alterar Gestor" onclick="addGestor()">
				<?php endif; ?>
				<img src="../imagens/excluir.gif" class="middle link" title="Limpar Gestor" onclick="limparGestor()">
				<input type="hidden" name="entidgestor" id="entidgestor" value="<?=$entidgestor?>" />
			</td>
		</tr>-->
		<!-- 
		<tr>
			<td class="SubtituloDireita">Gestor do Contrato:</td>
			<td>
                            <?php

                                $sql_combo = "select
                                        u.usucpf as codigo,
                                        u.usunome as descricao
                                from
                                        seguranca.perfilusuario pu
                                inner join
                                        seguranca.usuario u ON u.usucpf = pu.usucpf
                                where
                                        pu.pflcod = " . PERFIL_GESTOR_CONTRATO;

                                $db->monta_combo("gestorcpf",$sql_combo,'S',"-- Selecione --",'','','', '', 'N', 'gestorcpf','', '', $gestorcpf);

                            ?>
			</td>
		</tr>
		 -->
		<tr>
			<td class="SubtituloDireita">Data de Início:</td>
			<td>
				<?=campo_data2("dtinicio","N","S","Data","##/##/####")?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data de Término:</td>
			<td>
				<?=campo_data2("dtfim","N","S","Data","##/##/####")?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Situação:</td>
			<td>
				<?php $sql = "	select
									tscid as codigo,
									tscdsc as descricao
								from
									fabrica.tiposituacaocontrato
								where
									tscstatus = 'A'
								order by
									tscdsc"; ?>
				<?=$db->monta_combo("tscid",$sql,"S","Selecione...","","","","","N") ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarContrato()" value="Pesquisar" >
				<input type="button" onclick="novoContrato()" value="Novo Contrato" >
			</td>
		</tr>
	</table>
</form>
<?php $contrato->showListaContratos(); ?>