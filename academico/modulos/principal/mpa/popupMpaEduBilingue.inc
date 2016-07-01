<?php
$perfis = pegaPerfilGeral();
$habilita = 'N';
$habilita1 = 'S';
if(in_array(PERFIL_ADMINISTRADOR,$perfis)||$db->testa_superuser()){
	$habilita = 'S';
	$habilita1 = 'N';
};
function salvar(){
     
    global $db;
   
    if( $_REQUEST['edbid'] ){
    	$sql = "
            UPDATE academico.educacaobilingue SET
                edbnumvagas = '".$_POST['edbnumvagas']."',
                edbprevisto = '".$_POST['edbprevisto']."',
                edbrealizado = '".$_POST['edbrealizado']."',
                edbmodalidade = '".$_POST['edbmodalidade']."'
            WHERE edbid = {$_REQUEST['edbid']}";
    }else{
		$sql = "INSERT INTO academico.educacaobilingue (entidunidade,cebid,edbmodalidade,edbnumvagas, edbprevisto, edbrealizado) 
				VALUES('{$_SESSION['academico']['entid']}',".$_POST['cebid'].",'".$_POST['edbmodalidade']."','".$_POST['edbnumvagas']."','".$_POST['edbprevisto']."','".$_POST['edbrealizado']." ')";
    }
	$db->executar($sql);
	$db->commit();
?>
	<script>
		window.opener.location.href = window.opener.location.href;
		window.close();
	</script>
<?php
}

if($_REQUEST['req']){
	$_REQUEST['req']();
	die();
}

monta_titulo('Educação Bilíngue', '');

if( $_REQUEST['edbid'] ){
	$sql = "SELECT
				*
			FROM
				academico.educacaobilingue
			WHERE
				edbid = {$_REQUEST['edbid']}";
	$resposta = $db->pegaLinha($sql);
	extract($resposta);
}

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
});
</script>
<form name="form" id="form" method="POST">
	<input type="hidden" value="" name="req" id="req" />
	<input type="hidden" value="<?=$edbid ?>" name="edbid" id="edbid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td valign="top" class="SubtituloEsquerda" width="20%" >
				Curso: 
			</td>
			<td>
				<?php 
						$sql = "select
								
									cebid as codigo,
									cebdsc as descricao
								from
									academico.cursoeducbilingue";
	                $db->monta_combo('cebid',$sql,'S','Selecione',$acao,$opc,'','160','S', 'cebid', $return = false,$cebid,$title= null);
            	?>
			</td>
		</tr>
		<tr>
		<td class="SubtituloEsquerda">
		Modalidade:
		</td>
			<td>
				<input type="radio" name="edbmodalidade" value="p" <?=$edbmodalidade == 'p' ? 'checked' : ''?>/> Preferêncial
				<input type="radio" name="edbmodalidade" value="d" <?=$edbmodalidade == 'd' ? 'checked' : ''?>/> A Distância
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				N° de Vagas:
			</td>
			<td>
			<?=campo_texto('edbnumvagas','S',"S",'Modelo',10,20,'','', '', '', '', 'id="edbnumvagas"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda" colspan="2">
				Funcionamento
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Previsto:
			</td>
			<td>
				<?=campo_texto('edbprevisto','N',"$habilita",'Modelo',10,20,'####.#','', '', '', '', 'id="edbprevisto"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Realizado
			</td>
			<td>
				<?=campo_texto('edbrealizado','N',"$habilita1",'Modelo',10,20,'####.#','', '', '', '', 'id="edbrealizado"');?>
			</td>
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