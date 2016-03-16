<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."www/catalogocurso/_funcoes.php";

define("HOSPITALUNIV", 16);
define('HOSPITALFEDE', 93);

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if ($_REQUEST["resp"]){
	$resp = $_REQUEST["resp"];
	atribui($usucpf, $pflcod, $resp);
}

/**
 * Função que lista os hospitais
 *
 */
function lista(){
	$db = new cls_banco();
	
	$sql = "SELECT 		pus.pflcod 
			FROM		seguranca.perfilusuario pus
			INNER JOIN 	seguranca.perfil pfl ON pus.pflcod = pfl.pflcod
			WHERE		sisid = {$_SESSION['sisid']} AND pflstatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'";
	
	$perfils = $db->carregarColuna($sql);
	
	if( !$db->testa_superuser() && !in_array(665,$perfils) ){
		$arrCoords = recuperaCoordenacaoResponssavel();
	}
	
	if(count($arrCoords)>0){
		$coorid = implode(",",$arrCoords);
		$aryWhere[] =  "coordid IN ({$coordid})";
	}
	
	$aryWhere[] = "coorano = {$_SESSION['exercicio']}";
	
	$sql = "SELECT 		coordid AS codigo, 
						coordsigla AS sigla,
						coorddesc AS descricao 
			FROM		catalogocurso2014.coordenacao
						".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : ''). "";
	
	// SQL para buscar estados existentes
	$reg = $db->carregar($sql);
	$count = count($reg);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $reg[$i]["codigo"];
		$descricao = $reg[$i]["descricao"];
		$entsig    = $reg[$i]["sigla"];
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"cod\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"desc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">
					".$codigo."
				</td>
				<td>
					".$entsig."
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}
			
}

function atribui($usucpf, $pflcod, $resp){
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE catalogocurso2014.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND coordid IS NOT NULL");
	if ($resp[0]){
		foreach($resp as $tipo) {
			$dadosur = $db->carregar("SELECT * FROM catalogocurso2014.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND coordid = '{$tipo}'");
			if($dadosur) {
				// Se existir registro atualizar para ativo
				$db->carregar("UPDATE 	catalogocurso2014.usuarioresponsabilidade
   							   SET 		rpustatus = 'A', rpudata_inc = NOW()
 							   WHERE 	usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND coordid = '{$tipo}'");
			} else {
				// Se não existir, inserir novo
				$db->executar("INSERT INTO catalogocurso2014.usuarioresponsabilidade(pflcod, usucpf, coordid, rpustatus, rpudata_inc)
    						   VALUES ('{$pflcod}','{$usucpf}','{$tipo}','A',NOW());");
			}
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Operação realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

function buscaAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT 		c.coordid AS codigo, 
						c.coordsigla||' - '||c.coorddesc AS descricao 
			FROM 		catalogocurso2014.coordenacao c
			INNER JOIN 	catalogocurso2014.usuarioresponsabilidade u ON u.coordid = c.coordid
			WHERE		u.rpustatus = 'A' AND u.usucpf = '{$usucpf}' AND u.pflcod = {$pflcod} AND c.coorano = {$_SESSION['exercicio']}";
	
	$RS = @$db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$codigo\">$codigo - $descricao</option>";		
			}
		}
	} else {
		print '<option value="">Clique no estado selecionar.</option>';
	}
} ?>

<?php flush();?>

<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Estados</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione o tipo de ensino</strong></td>		
						</tr>
					</thead>
					<?php lista(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_coordenacao.php" method="post">
			<input type="hidden" name="usucpf" value="<?php echo $usucpf; ?>">
			<input type="hidden" name="pflcod" value="<?php echo $pflcod; ?>">
			<select multiple size="8" name="resp[]" id="resp" style="width:500px;" class="CampoEstilo">
				<?php buscaAtribuido($usucpf, $pflcod); ?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="javascript">
	var campoSelect = document.getElementById("resp");
	
	if (campoSelect.options[0].value != ''){
		for(var i=0; i<campoSelect.options.length; i++){
			document.getElementById(campoSelect.options[i].value).checked = true;
		}
	}
	
	function abreconteudo(objeto){
		if (document.getElementById('img'+objeto).name=='+'){
			document.getElementById('img'+objeto).name='-';
		    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
			document.getElementById(objeto).style.visibility = "visible";
			document.getElementById(objeto).style.display  = "";
		} else {
			document.getElementById('img'+objeto).name='+';
		    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
			document.getElementById(objeto).style.visibility = "hidden";
			document.getElementById(objeto).style.display  = "none";
		}
	}
	
	
	
	function retorna(objeto){
		var cod = document.getElementsByName('cod');
		var desc = document.getElementsByName('desc');
		tamanho = campoSelect.options.length;
		if (campoSelect.options[0].value=='') {tamanho--;}
		if (cod[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(desc[objeto].value, cod[objeto].value, false, false);
			sortSelect(campoSelect);
		} else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (cod[objeto].value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Coordenação.', '', false, false);}
			sortSelect(campoSelect);
		}
	}
	
	function moveto(obj) {
		if (obj.options[0].value != '') {
			if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
				abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
			}
			document.getElementById(obj.value).focus();
		}
	}
</script>