<?php

if ($_POST['requisicao'] == 'salvarDados') {
    if (is_array($_POST['mtpvalor']) && $_POST['mtpvalor']) {
        $sql = 'delete from projovemurbano.metasdoprograma where cmeid = ' . $_REQUEST['cmeid'] . ' and tpmid in (7,10,13);';
        foreach ($_POST['mtpvalor'] as $k => $v) {
            //fazer if
            
            if($_SESSION['projovemurbano']['ppuid'] == 2){
                $sql .= "insert into projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor, pjuid)
                                values ({$k}, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, '{$v}', '{$_SESSION['projovemurbano']['pjuid']}');";

            } else {
                $sql .= "insert into projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor)
                                values ({$k}, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, '{$v}');";
            }
        }
        
        if ($sql) {
            $db->executar($sql);
            $db->commit();
            $db->sucesso('principal/termoAdesao', '');
        }
    }
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', montaTituloEstMun());

$habil = 'N';
$size  = 20;

if($_SESSION['projovemurbano']['muncod']){
	$cmecodibge = $_SESSION['projovemurbano']['muncod'];
}else{	
	$cmecodibge = $db->pegaUm("select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'");
}

//$sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";

//

if($_SESSION['projovemurbano']['ppuid'] == 2){
    
    if($_SESSION['projovemurbano']['muncod']){
        
        $sql = "SELECT cam.* FROM projovemurbano.cargameta cam
                LEFT JOIN territorios.municipio mun ON (cam.cmecodibge = mun.muncod::INT)
                LEFT JOIN projovemurbano.projovemurbano ppu ON cam.cmecodibge = ppu.muncod::INT
                where 
                	cam.cmecodibge = '{$cmecodibge}' 
                	and ppu.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
                	and cam.ppuid = {$_SESSION['projovemurbano']['ppuid']}
                	AND pjuid = {$_SESSION['projovemurbano']['pjuid']}";
        
    }else {
        
        $sql = "SELECT cam.* FROM projovemurbano.cargameta cam
                LEFT JOIN territorios.estado est ON (cam.cmecodibge = est.estcod::INT)
                LEFT JOIN projovemurbano.projovemurbano ppu ON cam.cmecodibge = est.estcod::INT
                where 
                	cam.cmecodibge = '{$cmecodibge}' 
                	and ppu.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
                	and cam.ppuid = {$_SESSION['projovemurbano']['ppuid']}
                	AND pjuid = {$_SESSION['projovemurbano']['pjuid']}";
    
    }
} else {
    $sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
}

$rsCargaMeta = $db->pegaLinha($sql);

$sql = "select 
			t.tpmid,
			mtpvalor,
			tpmdescricao,
            pjuid
		from 
			projovemurbano.tipometadoprograma t
		left join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid  
													  AND cmeid = {$rsCargaMeta['cmeid']}
													  AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
		where
			t.tpmid in (7,10,13)";
          
$rsMetas = $db->carregar($sql);

//ver($rsCargaMeta['cmemeta']);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			erro=0;			
			$.each($("[name^='mtpvalor']"), function(i,v){				
				if($(v).val() == ''){
					erro++;
					$(v).focus();
					return false;
				}
			});

			if(erro>0){
				alert('Preencha todos os campos!');
				return false;
			}

			total = recuperaTotalMetas();

			if(parseInt(total) != parseInt($('[name=cmemeta]').val())){
				alert('O total das metas deve ser igual a Meta Total!');
				return false;
			}
				
			$('[name=requisicao]').val('salvarDados');
			$('#formulario').submit();		
		});

	});

	function recuperaTotalMetas()
	{
		total=0;
		$.each($("[name^='mtpvalor']"), function(i,v){
			if(parseInt($(v).val())>0)
				total = parseInt(total)+parseInt($(v).val());
		});

		return total;
	}
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="cmeid" value="<?=$rsCargaMeta['cmeid']; ?>" />	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="subtituloDireita" width="250">Meta Total:</td>
			<td>
				<!-- mtpvalor[4] -->
				<div id="valor_total" style="float:left;"><b><?=formata_numero($rsCargaMeta['cmemeta']); ?></b></div><div style="float:left">&nbsp;Estudantes</div>
				<input type="hidden" name="cmemeta" id="cmemeta" value="<?=$rsCargaMeta['cmemeta']; ?>" />
			</td>
		</tr>
		<?php foreach($rsMetas as $dados): ?>
			<tr>
				<td class="subtituloDireita"><?=$dados['tpmdescricao']; ?>:</td>
				<td><?php echo campo_texto('mtpvalor['.$dados['tpmid'].']', 'S', $habil, '', $size, '', '######', '', '', '', '', '', '', $dados['mtpvalor']); ?>&nbsp;Estudantes</td>
			</tr>
		<?php endforeach; ?>
<!-- 		<tr> 
 			<td class="subtituloDireita"></td> 
 			<td><input type="button" value="Salvar" id="btnSalvar" /></td> 
 		</tr> -->
	</table>
</form>