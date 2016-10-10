<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if(is_array($_POST['ugsresp']) && @count($_POST['ugsresp']) > 0) {


    
    $sql = "UPDATE pes.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$_POST['usucpf']."' AND pflcod='".$_POST['pflcod']."'";
    $db->executar($sql);

    if($_POST['ugsresp']) {
        foreach($_POST['ugsresp'] as $count => $entcodigo) {

            if($entcodigo){
                $ultimoacesso = '0' == $count ? 't' : 'f';

                $sql = "INSERT INTO pes.usuarioresponsabilidade(
                        pflcod, usucpf, rpustatus, rpudata_inc, entcodigo, rpuultimoacesso)
                        VALUES ('".$_POST['pflcod']."', '".$_POST['usucpf']."', 'A', NOW(), '".$entcodigo."', '$ultimoacesso');";

                $db->executar($sql);
            }
        }
    }
    
    $db->commit();

    echo "<script language=\"javascript\">
            alert(\"Operação realizada com sucesso!\");
            opener.location.reload();
            self.close();
          </script>";

}


/**
 * Função que lista os hospitais
 **/
function listaEntidades(){
	$db = new cls_banco();

	// SQL para buscar estados existentes
	$sql = "select     distinct
                ent.entcodigo,
                ent.uorcodigo,
                ent.entnome,
                ' (' || ent.uorcodigo || ') ' || ent.entnome as entidade
            from pes.pesentidade ent
                inner join pes.pesunidadeorcamentaria uor on uor.uorcodigo = ent.uorcodigo
            where uor.orgcodigo = '". CODIGO_ORGAO_SISTEMA. "'
            order by ent.entnome ";
	$ugs = $db->carregar($sql);

	$count = count($ugs);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $ugs[$i]["entcodigo"];
		$descricao = $ugs[$i]["entidade"];
		if (fmod($i,2) == 0){
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}

		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"ungcod\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"ungdsc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
				<td>".$descricao."</td>
			</tr>";
	}

}

function atribuiUgs($usucpf, $pflcod, $ungcods){
	$db = new cls_banco();

	$data = date("Y-m-d H:i:s");

	$db->executar("UPDATE elabrev.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod IS NOT NULL");

	if ($ungcods[0]){
		foreach($ungcods as $ungcod) {
			$dadosur = $db->carregar("SELECT * FROM elabrev.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod = '". $ungcod ."'");

				$uo = $db->pegaUm("SELECT unicod FROM public.unidadegestora WHERE ungcod = '". $ungcod ."'");

			if($dadosur) {

				// Se existir registro atualizar para ativo
				$db->executar("UPDATE elabrev.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW(), unicod='".$uo."'
 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod = '". $ungcod ."'");
			} else {
				// Se não existir, inserir novo
				$db->executar("INSERT INTO elabrev.usuarioresponsabilidade(
            				   pflcod, usucpf, unicod, ungcod, rpustatus, rpudata_inc, prsano)
    						   VALUES ('". $pflcod ."', '". $usucpf ."', '".$uo."', '". $ungcod ."', 'A', NOW(), '{$_SESSION['exercicio']}');");
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

function buscaUgsAtribuido($usucpf, $pflcod){

	$db = new cls_banco();

	$sql = "select distinct
                ent.entcodigo as codigo,
                ent.uorcodigo,
                ent.entnome,
                ' (' || ent.uorcodigo || ') ' || ent.entnome as descricao
            from pes.pesentidade ent
                inner join pes.pesunidadeorcamentaria uor on uor.uorcodigo = ent.uorcodigo
                inner join pes.usuarioresponsabilidade ur on ur.entcodigo = ent.entcodigo
            where uor.orgcodigo = '". CODIGO_ORGAO_SISTEMA. "'
            and ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = '$pflcod'
            order by ent.entnome ";

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
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades Gestoras</title>
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
					<?php listaEntidades(); ?>
				</table>
			</form>
		</div>

		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_entidade.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="ugsresp[]" id="ugsresp" style="width:500px;" class="CampoEstilo">
				<?php
					buscaUgsAtribuido($usucpf, $pflcod);
				?>
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
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript">

var campoSelect = document.getElementById("ugsresp");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}

function abreconteudo(objeto){
	if (document.getElementById('img'+objeto).name=='+'){
		document.getElementById('img'+objeto).name='-';
	    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
		document.getElementById(objeto).style.visibility = "visible";
		document.getElementById(objeto).style.display  = "";
	}else{
		document.getElementById('img'+objeto).name='+';
	    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
		document.getElementById(objeto).style.visibility = "hidden";
		document.getElementById(objeto).style.display  = "none";
	}
}

function retorna(objeto){

	tamanho = campoSelect.options.length;

	if (campoSelect.options[0].value=='') {
		tamanho--;
	}

	if (document.formulario.ungcod[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.ungdsc[objeto].value, document.formulario.ungcod[objeto].value, false, false);
		sortSelect(campoSelect);
	}else{
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.ungcod[objeto].value == campoSelect.options[i].value){
				campoSelect.options[i] = null;
			}
		}

		if (!campoSelect.options[0]){
			campoSelect.options[0] = new Option('Clique na UG.', '', false, false);
		}
		sortSelect(campoSelect);
	}
}

function moveto(obj){
	if (obj.options[0].value != ''){
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();
	}
}

</script>
<?php die; ?>