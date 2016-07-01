<?php
$perfis = pegaPerfilGeral();
$habilita = 'N';
if(in_array(PERFIL_ADMINISTRADOR,$perfis)||$db->testa_superuser()){
	$habilita = 'S';
};
function salvar(){

	global $db;
    
    extract($_POST);
    
    $adspercano2013 = $adspercano2013 ? $adspercano2013 : '0';
    $adspercano2014 = $adspercano2014 ? $adspercano2014 : '0';
    $adspercano2015 = $adspercano2015 ? $adspercano2015 : '0';
    $adspercano2016 = $adspercano2016 ? $adspercano2016 : '0';
    
    if( ($adeid !='' && isset($adeid)) || ($adsid!='' && isset($adsid) ) ){
    
    	$sql = " UPDATE academico.adesaoebserh SET
    				taeid2013 = $taeid2013, 
    				taeid2014 = $taeid2014, 
    				taeid2015 = $taeid2015,
    				taeid2016 = $taeid2016
    			WHERE
    				adeid = {$_REQUEST['adeid']};
    			UPDATE academico.adesaosisu SET
    				adspercano2013 = '".str_replace(",",".",$adspercano2013)."', 
    				adspercano2014 = '".str_replace(",",".",$adspercano2014)."', 
    				adspercano2015 = '".str_replace(",",".",$adspercano2015)."',
    				adspercano2016 = '".str_replace(",",".",$adspercano2016)."',
    				adsdscsit2013 = '".$_POST['adsdscsit2013']."',
    				adsdscsit2014 = '".$_POST['adsdscsit2014']."',
    				adsdscsit2015 = '".$_POST['adsdscsit2015']."',
    				adsdscsit2016 = '".$_POST['adsdscsit2016']."'
    			WHERE
    				adsid = {$_REQUEST['adsid']}";
    }else{
   
		$sql = "INSERT INTO academico.adesaoebserh (entid,taeid2013,taeid2014,taeid2015,taeid2016) 
				VALUES('{$_SESSION['academico']['entid']}',$taeid2013,$taeid2014,$taeid2015,$taeid2016); 	
				
				INSERT INTO academico.adesaosisu (entid,adspercano2013,adspercano2014,adspercano2015,adspercano2016,adedscsit2013,adedscsit2014,adedscsit2015,adedscsit2016) 
				VALUES('{$_SESSION['academico']['entid']}','".str_replace(",",".",$adspercano2013)."','".str_replace(",",".",$adspercano2014)."','".str_replace(",",".",$adspercano2015)."','".str_replace(",",".",$adspercano2016)."','".$_POST['adsdscsit2013']."','".$_POST['adsdscsit2014']."','".$_POST['adsdscsit2015']."','".$_POST['adsdscsit2016']."')";
    }
     
    $db->executar($sql);
	
	$db->commit();
?>
	<script>
		window.location.href = window.location.href;
		window.close();
	</script>
<?php
}

if($_REQUEST['req']){
	$_REQUEST['req']();
	die();
}

monta_titulo('Adesão(SISU-EBSERH)', '');

$sql = "SELECT
			*
		FROM
			academico.adesaoebserh
		WHERE
			entid = {$_SESSION['academico']['entid']}";
			
$resposta = $db->pegaLinha($sql);
$resposta = $resposta ? $resposta : Array();
extract($resposta);

$sql = "SELECT
		*
	FROM
		academico.adesaosisu
	WHERE
		entid = {$_SESSION['academico']['entid']}";
			
$resposta1 = $db->pegaLinha($sql);
$resposta1 = $resposta1 ? $resposta1 : Array();
extract($resposta1);

?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
$(document).ready(function(){
    $('.salvar').click(function(){
    	var erro = false;
    	$('.obrigatorio').each(function(){
    		if( $(this).val() == '' ){
    			erro = true;
    			$(this).focus();
    			return false;
    		}
    	})
    	
    	if( erro ){
    		alert('Campo obrigatório.');
    		return false;
    	}
        $('#req').val('salvar');
        $('#form').submit();
    });
    
    $('[type="text"]').keyup();
});
</script>
<form name="form" id="form" method="POST">
	<input type="hidden" value="" name="req" id="req" />
	<input type="hidden" value="<?=$adeid ?>" name="adeid" id="adeid" />
	<input type="hidden" value="<?=$adsid ?>" name="adsid" id="adsid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td valign="top" class="SubtituloEsquerda" width="20%" >
				EBESRH
			</td>
		</tr>	
		<tr>
			<td valign="center" class="SubtituloCentro" width="20%" rowspan="4" >
			Situação
			</td>	
			<td valign="top" >
			2013
				<?php 
						$sql = "select
									taeid as codigo,
									taedsc as descricao
								from
									academico.tipoadesaoifes";
	                $db->monta_combo('taeid2013',$sql,"$habilita",'Selecione',$acao,$opc,'','160','N', 'taeid2013', $return = false,$taeid2013,$title= null);
            	?>
			</td>
		</tr>
		<tr>
			<td valign="top" >
			2014
				<?php 
						$sql = "select
									taeid as codigo,
									taedsc as descricao
								from
									academico.tipoadesaoifes";
	                $db->monta_combo('taeid2014',$sql,"$habilita",'Selecione',$acao,$opc,'','160','N', 'taeid2014', $return = false,$taeid2014,$title= null);
            	?>
			</td>
		</tr>
		<tr>
			<td valign="top" >
			2015
				<?php 
						$sql = "select
									taeid as codigo,
									taedsc as descricao
								from
									academico.tipoadesaoifes";
	                $db->monta_combo('taeid2015',$sql,"$habilita",'Selecione',$acao,$opc,'','160','N', 'taeid2015', $return = false,$taeid2015,$title= null);
            	?>
			</td>
		</tr>
		<tr>
			<td valign="top" >
			2016
				<?php 
						$sql = "select
									taeid as codigo,
									taedsc as descricao
								from
									academico.tipoadesaoifes";
	                $db->monta_combo('taeid2016',$sql,"$habilita",'Selecione',$acao,$opc,'','160','N', 'taeid2016', $return = false,$taeid2016,$title= null);
            	?>
			</td>
		</tr>
		<tr>
			<td valign="top" class="SubtituloEsquerda" width="20%" >
				SISU
			</td>
		</tr>	
		<tr>
				<td valign="center" class="SubtituloCentro" width="20%" rowspan="4">
				Situação
				</td>	
				<td valign="top" >
				2013<?=campo_texto("adspercano2013",'N',"$habilita",'',5,4,'##,##','', '', '', '', 'id="adspercano2013"','','');?>%
					<?=campo_textarea( 'adsdscsit2013', 'N', "$habilita", '', 80, 5, 500, '', 0, '', false, NULL, $dindsc) ?>
				</td>
			</tr>
			<tr>
				<td valign="top" align="left">
				2014<?=campo_texto("adspercano2014",'N',"$habilita",'',5,4,'##,##','', '', '', '', 'id="adspercano2014"','','');?>%
					<?=campo_textarea( 'adsdscsit2014', 'N',"$habilita", '', 80, 5, 500, '', 0, '', false, NULL, $dindsc) ?>
				</td>
			<tr>
				<td valign="top" >
				2015<?=campo_texto("adspercano2015",'N',"$habilita",'',5,4,'##,##','', '', '', '', 'id="adspercano2015"','','');?>%	
					<?=campo_textarea( 'adsdscsit2015', 'N', "$habilita", '', 80, 5, 500, '', 0, '', false, NULL, $dindsc) ?>
				</td>
			</tr>
			<tr>
				<td valign="top" >
				2016<?=campo_texto("adspercano2016",'N',"$habilita",'',5,4,'##,##','', '', '', '', 'id="adspercanot2016"','','');?>%
					<?=campo_textarea( 'adsdscsit2016', 'N', "$habilita", '', 80, 5, 500, '', 0, '', false, NULL, $dindsc) ?>
				</td>
			</tr>
		</tr>
		<tr>
			<td>
			</td>
 			<td>
	        	<input class="salvar" type="button" value="Salvar">
	        </td>
   	 	</tr>
	</table>
</form>