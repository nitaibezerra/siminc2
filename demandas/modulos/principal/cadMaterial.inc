<?php

if($_REQUEST['filtraMaterial'] && $_REQUEST['ordid']){
	
	// montando o select com os novos dados
	$sql = "SELECT 
				mtrid as codigo,
				mtrdsc as descricao
			FROM 
				demandas.material
			WHERE
				mtrstatus = 'A'
			AND 
				ordid = ".$_REQUEST['ordid']."
			ORDER BY descricao";
	$materiais = $db->carregar($sql);
	$html = "<option value=''>Selecione...</option>";
	
	if($materiais){
		foreach ($materiais as $material){
			$html .= "<option value='{$material['codigo']}'>{$material['descricao']}</option>";
		}
	}
		
	echo $html;
	exit();
}

if( $_POST['pesquisa'] ){ // início da pesquisa
	
	$arCondicao = array();

	if( $_POST['ordid'] ){
		$arCondicao[] = "o.ordid = {$_POST['ordid']}";
	}
	
	if( $_POST['material'] ){
		$arCondicao[] = "dm.mtrid = {$_POST['material']}";
	}
	
	if( $_POST['tipo'] ){
		$arCondicao[] = "dtm.tpmid = {$_POST['tipo']}";
	}
	
	if( $_POST['quantidade'] ){
		$arCondicao[] = "dcm.ctmqtd = {$_POST['quantidade']}";
	}
	
	if( $_POST['debito'] == 1 ){
		$arCondicao[] = "dtc.tpcdebito = 'f'";
	}elseif ($_POST['debito'] == 2){
		$arCondicao[] = "dtc.tpcdebito = 't'";
	}
	
	// fim da pesquisa
}elseif ( isset($_POST['material']) && isset($_POST['tipo']) && isset($_POST['quantidade']) && isset($_POST['debito']) ){ // salvando no banco de dados
	
	$sql = "INSERT INTO 
				demandas.controlematerial
				(tpcid, tpmid, ctmqtd, ctmstatus, usucpf, ctmdatahora)
    		VALUES
    			({$_POST['debito']}, {$_POST['tipo']}, {$_POST['quantidade']}, 'A', '{$_SESSION['usucpf']}', now());";
	
	$db->carregar($sql);
	$db->commit();
	echo "<script>
				alert('Dados inseridos com sucesso.');
				location.href = 'demandas.php?modulo=principal/cadMaterial&acao=A';
		 </script>";
	exit();
	// fim do salvar no banco de dados
	
}elseif ( isset($_POST['material']) ) { // ajax dos materiais
	header("Content-Type: text/html; charset=ISO-8859-1");
	
	$sql = "SELECT 
				tpmid as codigo,
				tpmdsc as descricao
			FROM 
				demandas.tipomaterial
			WHERE
				tpmstatus = 'A'
				AND mtrid = {$_POST['material']}
			ORDER BY descricao";
	
	$tipos = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	if( is_array($tipos) ){
		foreach ($tipos as $tipo) {
			$html .= "<option value='{$tipo['codigo']}'>{$tipo['descricao']}</option>";
		}
		
	}
	
	echo $html;
	exit();
	// fim do ajax dos materiais
	
}elseif ( $_REQUEST['excluir'] && $_REQUEST['id'] ){ // ajax da função excluir
	
	$sql = "UPDATE 
				demandas.controlematerial
			SET 
				ctmstatus='I'
			WHERE
				ctmid={$_POST['id']}";
	
	$db->carregar($sql);
	$db->commit();
	exit();
	// fim do ajax da função excluir
	
}elseif ( $_REQUEST['excluir'] && $_REQUEST['tpmid'] ){ // ajax excluindo Tipo de Material
	
	$sql = "UPDATE 
				demandas.tipomaterial
   			SET 
   				tpmstatus='I'
 			WHERE 
 				tpmid={$_REQUEST['tpmid']}";
	
	$db->carregar($sql);
	$db->commit();
	exit();
	// fim do ajax excluindo Tipo de Material
	
}elseif ( $_REQUEST['excluir'] && $_REQUEST['mtrid'] ){ // ajax excluindo Material
	
	$sql = "UPDATE
				demandas.material
   			SET 
   				mtrstatus='I'
 			WHERE 
 				mtrid={$_REQUEST['mtrid']}";
	
	$db->carregar($sql);
	$db->commit();
	exit();
	// fim do ajax excluindo Material
	
}elseif ( $_REQUEST['novo'] == 'InserirMaterial' ){ // formulário para inserir um novo material
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Inserir Material</title>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript">
$(document).ready(function() {

	$("#salvar").click(function () {

		if ( !$('#ordid').val() ){
			alert('Selecione uma Origem.');
			$('#ordid').focus();
			return false;
			
		}else if ( !$('#unmid').val() ){
			alert('Selecione uma Unidade de Medida.');
			$('#unmid').focus();
			return false;
			
		}else if( !$('#mtrdsc').val() ){
			alert('Preencha a Descrição do Material corretamente.');
			$('#mtrdsc').focus();
			return false;
			
		}else{
			//enviando o post
			$.post('demandas.php?modulo=principal/cadMaterial&acao=A', { ordid : $('#ordid').val(), unmid : $('#unmid').val(), mtrdsc : $('#mtrdsc').val(), novo : 'NovoMaterial' },
				function(data){
					$('#material', window.opener.document).html(data);
				});
			alert('Registro salvo com sucesso.');
			location.href = 'demandas.php?modulo=principal/cadMaterial&acao=A&novo=InserirMaterial';
			return true;
		}
		
	})

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este registro?') ){

			var mtrid = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadMaterial&acao=A&excluir=1', { mtrid : mtrid },
				function(){
					alert('Material excluido com sucesso.');
					location.href='demandas.php?modulo=principal/cadMaterial&acao=A&novo=InserirMaterial';
				});
			
		}
		
	})
	
});
</script>
</head>
<body>
<div id="debug"></div>
<form action="" method="post" name="formulario">
	<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%">
				<label class="TituloTela">Novo Material</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Origem</td>
			<td>
				<?php
					$sql = "SELECT
								ordid as codigo,
								orddescricao as descricao
							FROM 
								demandas.origemdemanda
							WHERE
								ordstatus = 'A'
								and ordid in (3,5,14)
							ORDER BY ordid";
					 echo $db->monta_combo("ordid",$sql, 'S','Selecione...', '', '', '', 250, 'S','ordid','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Unidade de Medida</td>
			<td>
				<?php
					$sql = "SELECT
								unmid as codigo,
								unmdsc as descricao
							FROM 
								demandas.unidademedida
							WHERE
								unmstatus = 'A'
							ORDER BY descricao";
					 echo $db->monta_combo("unmid",$sql, 'S','Selecione...', '', '', '', 250, 'S','unmid','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Descrição do Material</td>
			<td><input type="text" id="mtrdsc" name="mtrdsc" size="90" maxlength="250">&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."></td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0"><input type="button" value="Salvar" id="salvar"></td>
		</tr>
	</table>
</form>

<?php
	$sql = "SELECT 
				'<center>
					<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| dm.mtrid ||'\" title=\"excluir\" alt=\"excluir\" \">
				 </center>' as acao,
				dm.mtrdsc as material,
				dum.unmdsc as medida
			FROM 
				demandas.material dm
			INNER JOIN demandas.unidademedida dum ON dum.unmid = dm.unmid
			WHERE
				dm.mtrstatus = 'A'
				AND dum.unmstatus = 'A'
			ORDER BY mtrdsc";
	$cabecalho = array("Excluir", "Material", "Unidade de Medida");
	echo $db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');
?>

</body>
</html>
<?php
	exit();
	// fim do formulário de novo material
	
}elseif ( $_REQUEST['novo'] == 'NovoMaterial' && $_REQUEST['mtrdsc'] && $_REQUEST['unmid'] ){ // popup inserindo novo material

	$sql = "INSERT INTO 
				demandas.material
				(ordid, unmid, mtrdsc, mtrstatus)
			VALUES 
				({$_REQUEST['ordid']}, {$_REQUEST['unmid']}, '{$_REQUEST['mtrdsc']}', 'A')";
	
	
	$db->carregar( iconv("UTF-8", "ISO-8859-1", $sql) );
	$db->commit();
	
	// montando o select com os novos dados
	$sql = "SELECT 
				mtrid as codigo,
				mtrdsc as descricao
			FROM 
				demandas.material
			WHERE
				mtrstatus = 'A'
			ORDER BY descricao";
	$materiais = $db->carregar($sql);
	$html = "<option value=''>Selecione...</option>";
	
	foreach ($materiais as $material){
		$html .= "<option value='{$material['codigo']}'>{$material['descricao']}</option>";
	}
	
	echo $html;
	exit();
	// fim do popup inserindo novo material
	
}elseif ( $_REQUEST['novo'] == 'InserirTipo' ){ // início do formulario de novo tipo
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Inserir Tipo de Material</title>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript">
$(document).ready(function() {

	$("#salvar").click(function () {

		if ( !$('#ordid').val() ){
			alert('Selecione uma Origem.');
			$('#ordid').focus();
			return false;
			
		}else if ( !$('#mtrid').val() ){
			alert('Selecione um Material.');
			$('#mtrid').focus();
			return false;
			
		}else if( !$('#tpmdsc').val() ){
			alert('Preencha a Descrição do Tipo corretamente.');
			$('#tpmdsc').focus();
			return false;
			
		}else if ( !$('#tpmespecificacao').val() ){
			alert('Preencha a Especificação do Tipo corretamente.');
			$('#tpmespecificacao').focus();
			return false;
			
		}else{
			//enviando o post
			$.post('demandas.php?modulo=principal/cadMaterial&acao=A', { mtrid : $('#mtrid').val(), tpmdsc : $('#tpmdsc').val(), tpmespecificacao : $('#tpmespecificacao').val(), novo : 'NovoTipo' },
				function(data){
					// alterando o select do material para a mesma opção que o usuário cadastrou anteriormente
					$("#material option[value='"+$('#mtrid').val()+"']", window.opener.document).attr('selected', 'selected');
					// atualizando o select do tipo
					$('#tipo', window.opener.document).html(data);
				});
			alert('Registro salvo com sucesso.');
			location.href = 'demandas.php?modulo=principal/cadMaterial&acao=A&novo=InserirTipo';
			return true;
		}
		
	})
	
	//origem (filtra material)
	$("#ordid").change(function () {

		$('#mtrid').html('<option>Aguarde...</option>');
		$('#mtrid').attr('disabled', true); 
		
		//enviando o post
		$.post('demandas.php?modulo=principal/cadMaterial&acao=A', { ordid : $('#ordid').val(), filtraMaterial : 'ok' },
			function(data){
				// atualizando o select do tipo
				$('#mtrid').attr('disabled', false);
				$('#mtrid').html(data);
			});
		return true;

	})	

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este registro?') ){

			var tpmid = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadMaterial&acao=A&excluir=1', { tpmid : tpmid },
				function(){
					alert('Tipo de Material excluido com sucesso.');
					location.href='demandas.php?modulo=principal/cadMaterial&acao=A&novo=InserirTipo';
				});
			
		}
		
	})
	
});
</script>
</head>
<body>
<div id="debug"></div>
<form action="" method="post" name="frm">
	<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%">
				<label class="TituloTela">Novo Tipo de Material</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Origem</td>
			<td>
				<?php
					$sql = "SELECT
								ordid as codigo,
								orddescricao as descricao
							FROM 
								demandas.origemdemanda
							WHERE
								ordstatus = 'A'
								and ordid in (3,5,14)
							ORDER BY ordid";
					 echo $db->monta_combo("ordid",$sql, 'S','Selecione...', '', '', '', 250, 'S','ordid','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Material</td>
			<td>
				<?php
					$sql = "SELECT 
								mtrid as codigo,
								mtrdsc as descricao
							FROM 
								demandas.material
							WHERE
								mtrstatus = 'X'
							ORDER BY descricao";
					 echo $db->monta_combo("mtrid",$sql, 'S','Selecione...', '', '', '', 250, 'S','mtrid','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Descrição do Tipo</td>
			<td><input type="text" id="tpmdsc" name="tpmdsc" maxlength="250">&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Especificação</td>
			<td><?php echo campo_textarea( 'tpmespecificacao', 'N', 'S', '', '50', '10', '3000', '' , 0, '', false, NULL); ?></td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0"><input type="button" value="Salvar" id="salvar"></td>
		</tr>
	</table>
</form>

<?php
	$sql = "SELECT
				'<center>
					<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| dtm.tpmid ||'\" title=\"excluir\" alt=\"excluir\" \">
				 </center>' as acao,
				 dm.mtrdsc as material_descricao,
				dtm.tpmdsc as tipo_material_dsc,
				dtm.tpmespecificacao as especificacao
			FROM
				demandas.tipomaterial dtm
			INNER JOIN
				demandas.material as dm ON dm.mtrid = dtm.mtrid
			WHERE
				dtm.tpmstatus = 'A'
				AND dm.mtrstatus = 'A'
			ORDER BY dtm.tpmdsc"; 
	
 	$cabecalho = array("Excluir", "Material", "Tipo de Material", "Especificação");
	echo $db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');
?>

</body>
</html>
<?php
	exit();
	// fim do formulario de novo tipo
	
}elseif ( $_REQUEST['novo'] == 'NovoTipo' && $_REQUEST['tpmdsc'] && $_REQUEST['mtrid'] && $_REQUEST['tpmespecificacao'] ){ // inserindo
	
	$sql = "INSERT INTO 
				demandas.tipomaterial
				(mtrid, tpmdsc, tpmstatus, tpmespecificacao)
			VALUES
				({$_REQUEST['mtrid']}, '{$_REQUEST['tpmdsc']}', 'A', '{$_REQUEST['tpmespecificacao']}');";
	
	$db->carregar( iconv("UTF-8", "ISO-8859-1", $sql) );
	$db->commit();
	
	$sql = "SELECT 
				tpmid as codigo,
				tpmdsc as descricao
			FROM 
				demandas.tipomaterial
			WHERE
				tpmstatus = 'A'
				AND mtrid = {$_REQUEST['mtrid']}
			ORDER BY descricao";
	
	$tipos = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	if( is_array($tipos) ){
		foreach ($tipos as $tipo) {
			$html .= "<option value='{$tipo['codigo']}'>{$tipo['descricao']}</option>";
		}
		
	}
	echo $html;
	
	exit();
	// fim do inserindo
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$titulo_modulo = "Cadastro de Material";
monta_titulo( $titulo_modulo, "&nbsp;");

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {

	// ajax do origem
	$("#ordid").change(function () {

		$('#material').html('<option>Aguarde...</option>');

		var ordid = $("#ordid").val();

		//se material não possuir valor então eu saio da função
		if(!ordid){
			$('#material').attr('disabled', true);
			$('#material').html('');
			return false;
		}
		
		//enviando o post
		$.post('demandas.php?modulo=principal/cadMaterial&acao=A', { ordid : ordid, filtraMaterial : 'ok' },
			function(data){
				$('#material').html(data);
				$('#material').attr('disabled', false);
			});
		
	})
		
	// ajax do material
	$("#material").change(function () {

		$('#tipo').html('<option>Aguarde...</option>');

		var material = $("#material").val();

		//se material não possuir valor então eu saio da função
		if(!material){
			$('#tipo').attr('disabled', true);
			$('#tipo').html('');
			return false;
		}
		
		//enviando o post
		$.post('demandas.php?modulo=principal/cadMaterial&acao=A', { material : material },
			function(data){
				$('#tipo').html(data);
				$('#tipo').attr('disabled', false);
			});
		
	})

	$("#salvar").click(function () {

		if( !$('select:#ordid option:selected').val() ){
			alert('Preencha a Origem corretamente.');
			$('#ordid').focus();
			return false;
			
		}else if( !$('select:#material option:selected').val() ){
			alert('Preencha o Material corretamente.');
			$('#material').focus();
			return false;
			
		}else if( !$('select:#tipo option:selected').val() ){
			alert('Preencha o Tipo corretamente.');
			$('#tipo').focus();
			return false;
			
		}else if( !$('#quantidade').val() ){
			alert('Preencha a Quantidade corretamente.');
			$('#quantidade').focus();
			return false;
			
		}else if( !$('#debito:checked').val() ){
			alert('Preencha o Débito corretamente.');
			$('#debito').focus();
			return false;
			
		}else{
			document.formulario.submit();
			return true;
		}
			
	})
	
	// botão pesquisar
	$("#pesquisar").click(function () {
		$('#debug').html('<input type="hidden" name="pesquisa" value="1">');
		document.formulario.submit();
	})
	
	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este registro?') ){

			var id = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadMaterial&acao=A&excluir=1', { id : id },
				function(){
					alert('Material excluido com sucesso.');
					location.href='demandas.php?modulo=principal/cadMaterial&acao=A';
				});
			
		}
		
	})

	$("[id^='Inserir']").click(function () {
		if( this.id == 'InserirTipo' ){
			window.open('demandas.php?modulo=principal/cadMaterial&acao=A&novo='+this.id,this.id,'width=530,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		}else{
			window.open('demandas.php?modulo=principal/cadMaterial&acao=A&novo='+this.id,this.id,'width=400,height=150,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		}
	})
});
</script>

<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<tr>
			<td class="SubTituloEsquerda">Origem:</td>
			<td>
				<?php
					$sql = "SELECT
								ordid as codigo,
								orddescricao as descricao
							FROM 
								demandas.origemdemanda
							WHERE
								ordstatus = 'A'
								and ordid in (3,5,14)
							ORDER BY ordid";
					 echo $db->monta_combo("ordid",$sql, 'S','Selecione...', '', '', '', 250, 'S','ordid','',$_REQUEST['ordid']);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Material:</td>
			<td>
				<?php
					if( $_REQUEST['ordid']  ){
						
						$sql = "SELECT 
								mtrid as codigo,
								mtrdsc as descricao
								FROM 
									demandas.material
								WHERE
									mtrstatus = 'A'
								AND ordid = {$_REQUEST['ordid']}
								ORDER BY descricao";
						
						echo $db->monta_combo("material",$sql, 'S','Selecione...', '', '', '', 250, 'S','material','',$_REQUEST['material']);
						
					}else{
				?>
				<select id="material" style="width: auto;" class="CampoEstilo" name="material" disabled="disabled">
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php } ?>				
				&nbsp;
				<a href="#" id="InserirMaterial">
					<img border="0" title="Inserir Material" alt="Inserir Material" style="cursor: pointer;" src="/imagens/gif_inclui.gif">
					Inserir Material
				</a>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Tipo:</td>
			<td>
				<?php
					if( $_REQUEST['material'] && $_REQUEST['tipo'] ){
						
						$sql = "SELECT 
									tpmid as codigo,
									tpmdsc as descricao
								FROM 
									demandas.tipomaterial
								WHERE
									tpmstatus = 'A'
									AND mtrid = {$_REQUEST['material']}
								ORDER BY descricao";
						
						echo $db->monta_combo("tipo",$sql, 'S','Selecione...', '', '', '', 250, 'S','tipo','',$_REQUEST['tipo']);
						
					}else{
				?>
				<select id="tipo" style="width: auto;" class="CampoEstilo" name="tipo" disabled="disabled">
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php } ?>
				&nbsp;
				<a href="#" id="InserirTipo">
					<img border="0" title="Inserir Tipo" alt="Inserir Tipo" style="cursor: pointer;" src="/imagens/gif_inclui.gif">
					Inserir Tipo
				</a>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Quantidade:</td>
			<td>
				<input type="text" name="quantidade" id="quantidade" maxlength="10" value="<?php echo $_REQUEST['quantidade']; ?>">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Entrada/Saída</td>
			<td>
			
				<?php

					$sql = "SELECT
								tpcid as codigo,
								tpcdsc as descricao
							FROM 
								demandas.tipocontrole";
					
					$tipocontroles = $db->carregar($sql);
					
					foreach ($tipocontroles as $tipocontrole) {
						// marcando o radio caso o usuário esteja fazendo uma pesquisa
						if( $_REQUEST['debito'] == $tipocontrole['codigo'] ){ 
							$html = 'checked="checked"'; 
						}else{
							$html = '';
						}
				?>
						<label><?php echo $tipocontrole['descricao']; ?>&nbsp;<input <?php echo $html; ?> type="radio" name="debito" id="debito" value="<?php echo $tipocontrole['codigo']; ?>"></label>
				<?php 	
					}
				?>
			
			</td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0">
				<input type="button" value="Salvar" id="salvar">
				&nbsp;
				<input type="button" value="Pesquisar" id="pesquisar">
			</td>
		</tr>
	</table>
	<div id="debug"></div>
</form>

<?php 


$sql = "SELECT 
			'<center>
				<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| dcm.ctmid ||'\" title=\"excluir\" alt=\"excluir\" \">
			 </center>' as acao,
			dm.mtrdsc as material,
			dtm.tpmdsc as tipo,
			dcm.ctmqtd as quantidade,
			dtc.tpcdsc as entradasaida,
			to_char(dcm.ctmdatahora, 'DD/MM/YYYY HH24:MI') as data,
			o.orddescricao as origem
		FROM 
			demandas.controlematerial dcm
		INNER JOIN 
			demandas.tipomaterial dtm ON dtm.tpmid = dcm.tpmid
		INNER JOIN
			demandas.material dm ON dm.mtrid = dtm.mtrid
		INNER JOIN
			demandas.origemdemanda o ON o.ordid = dm.ordid	
		INNER JOIN
			demandas.tipocontrole dtc ON dtc.tpcid = dcm.tpcid
		WHERE
			dcm.ctmstatus = 'A'" . ($arCondicao ? ' AND '.implode(' AND ', $arCondicao) : '');

$cabecalho = array( "Excluir", "Material", "Tipo", "Quantidade", "Entrada/Saída", "Data/Hora", "Origem" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>
