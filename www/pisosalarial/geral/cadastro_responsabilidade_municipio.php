<?php
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

// INICIO REGISTRO RESPONSABILIDADES
if(isset($_REQUEST['enviar'])) {

	$sql = "update
			 pisosalarial.usuarioresponsabilidade
			set
			 rpustatus = 'I'
			where
			 usucpf = '$usucpf'
			 and pflcod = $pflcod ";

	if(count($_POST['usuunidresp']) < 2){
    	if($_POST['usuunidresp'][0]){

    	    $sql = "DELETE FROM pisosalarial.usuarioresponsabilidade
    	            WHERE muncod NOT IN ('".implode("', '" , $_POST['usuunidresp'])."')
    	            AND usucpf = '{$usucpf}'
    	            AND pflcod = {$pflcod}";

    	    $db->executar($sql);

    		foreach($_POST['usuunidresp'] as $muncod){

    			$sql = "INSERT INTO pisosalarial.usuarioresponsabilidade (muncod, usucpf, rpustatus, rpudata_inc, pflcod)
    																   VALUES ('$muncod', '$usucpf', 'A',  now(), '$pflcod')";
    			$db->executar($sql);
    		}
    	}
    	$db->commit();
	}

	?>
<script>
    window.parent.opener.location.reload();
    self.close();
</script>
	<?
	exit(0);
}
?>
<html>
<head>
    <meta http-equiv="pragma" content="no-cache">
    <title>Estados e Municípios</title>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
    <link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<div align=center id="aguarde">
    <img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>
<?/*flush();*/?>
<div id="div_arvore" style="position:absolute;top:5px;overflow:auto;margin:0px;top:0px;height:350px;width:100%;">
<form name="formulario" method="post" action="">
    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
    	<thead>
    		<tr>
    			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
                    <strong>Selecione a(s) estado(s)</strong>
                </td>
    		</tr>
    		<tr>
    		<?
    		$cabecalho = 'Selecione a(s) estado(s)';
    		$sql = "
    			select
    				estuf, estuf, estdescricao
    			from territorios.estado
    			where estuf in ('AL', 'BA', 'CE', 'MA', 'PB', 'PE', 'PI', 'RN', 'AM', 'PA')
    			order by estuf, estdescricao";

    		$RS = @$db->carregar($sql);
    		$nlinhas = count($RS)-1;
    		$j = 0 ;
    		for ($i=0; $i<=$nlinhas;$i++)
    		{
    			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
    			?>
    		<tr bgcolor="<?=$cor?>">
    			<td width="20" align="right"><img src="/imagens/mais.gif"
    				id="<?=$estuf."_img" ?>" onclick="mostraEsconde('<?=$estuf?>')">&nbsp;</td>
    			<td align="left" style="color: blue;"><?=$estuf . ' - ' . $estdescricao?></td>
    		</tr>
    		<tr>
    			<td style="height: 0"></td>
    			<td style="height: 0">
        			<div id="<?=$estuf?>" style="display: none;">
        			<table width="100%" cellpadding="0" cellspacing="0" border="0">
        			<?
        			$sql = "select mundescricao, muncod from territorios.municipio where estuf = '$estuf' order by mundescricao";
        			$municipios = $db->carregar($sql);
        			foreach ($municipios as $municipio) {
        				if ($cor2 == '#e0e0e0') $cor2 = '#f4f4f4' ; else $cor2='#e0e0e0';
        				?>
        				<tr bgcolor="<?=$cor2?>">
        					<td align="left" style="border: 0">
                                <input type="checkbox" name="muncod" id="<?=$municipio['muncod']?>" value="<?=$municipio['muncod']?>" onclick="retorna( this, '<?= $municipio['muncod'] ?>', '<?= $estuf.' - '. addslashes( $municipio['mundescricao'] ) ?>' );" />
        						<?=$municipio['mundescricao']?>
                            </td>
        				</tr>
        				<? } ?>
        			</table>
        			</div>
    			</td>
    		</tr>
    		<? } ?>
    </table>
</form>
</div>
<div style="position:absolute;top:355px;">
<form name="formassocia" style="margin: 0px;" method="POST">
    <input type="hidden" name="usucpf" value="<?=$usucpf?>">
    <input type="hidden" name="pflcod" value="<?=$pflcod?>">
    <input type="hidden" name="enviar" value="">
    <select multiple size="8" onclick="mostraMunicipio(this);" name="usuunidresp[]" id="usuunidresp" style="width: 500px;" class="CampoEstilo">
	<?php
	$sql = "select
				distinct m.muncod as codigo, m.estuf||' - '||m.mundescricao as descricao
			from pisosalarial.usuarioresponsabilidade ur
			inner join territorios.municipio m on ur.muncod = m.muncod
	 		where ur.rpustatus='A'
	 		and ur.usucpf = '$usucpf'
	 		and ur.pflcod=$pflcod";

	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$descricao</option>";
			}
		}
	}
	?>
    </select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
    <tr bgcolor="#c0c0c0">
		<td align="left" style="padding: 3px;" colspan="3">
            <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok">
        </td>
	</tr>
</table>
</div>
</body>
<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript">

    document.getElementById('tabela').style.visibility = "hidden";
    document.getElementById('tabela').style.display  = "none";

    jQuery.noConflict();

    document.getElementById('aguarde').style.visibility = "hidden";
    document.getElementById('aguarde').style.display  = "none";
    document.getElementById('tabela').style.visibility = "visible";
    document.getElementById('tabela').style.display  = "";

    function mostraEsconde(estado){
    	var estadoAtual = document.getElementById(estado).style.display;
    	var objImagem = document.getElementById(estado+'_img');
    	if(estadoAtual == 'none'){
    		document.getElementById(estado).style.display = 'block';

    		objImagem.src = '/imagens/menos.gif';

    	}else{
    		document.getElementById(estado).style.display = 'none';
    		objImagem.src = '/imagens/mais.gif';
    	}
    }

    var campoSelect = document.getElementById("usuunidresp");

    if (campoSelect.options[0] && campoSelect.options[0].value != ''){
    	for(var i=0; i<campoSelect.options.length; i++)
    		{document.getElementById(campoSelect.options[i].value).checked = true;}
    }

    function enviarFormulario(){
    	document.formassocia.enviar.value=1;
    	document.formassocia.submit();
    }

    function mostraMunicipio(objSelect){
    	for( var i = 0; i < objSelect.options.length; i++ ){
    		if ( objSelect.options[i].value == objSelect.value ){
    			var estado = objSelect.options[i].innerHTML.substring(0,2);
    			break;
    		}
    	}
    	var estadoAtual = document.getElementById(estado).style.display;
    	if(estadoAtual != 'block'){
    		 mostraEsconde(estado);
    	}
    	document.getElementById(objSelect.value).focus();

    }

    function retorna( check, muncod, mundescricao ){
        if(jQuery('input[name=muncod]:checked').size() > 1){
            alert('É permitido selecionar somente um município.');
            jQuery('#'+muncod).attr('checked', false);
            return false;
        }
    	if ( check.checked ){
    		// põe
    		campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
    	}else{
    		// tira
    		for( var i = 0; i < campoSelect.options.length; i++ )
    		{
    			if ( campoSelect.options[i].value == muncod )
    			{
    				campoSelect.options[i] = null;
    			}
    		}
    	}
    	sortSelect( campoSelect );
    }
</script>