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
    if( $_REQUEST['pcmid'] ){
    	$sql = "
            UPDATE academico.procampo SET
                pcmnumvagas = '".$_POST['pcmnumvagas']."',
                pcmprevisto = '".$_POST['pcmprevisto']."',
                pcmrealizado = '".$_POST['pcmrealizado']."'
            WHERE pcmid = {$_REQUEST['pcmid']}";
    }else{
		$sql = "INSERT INTO academico.procampo (entidunidade, entidcampus,pcmnumvagas, pcmprevisto, pcmrealizado) 
            VALUES('{$_SESSION['academico']['entid']}', '{$_POST['entidcampus']}', {$_POST['pcmnumvagas']}, '{$_POST['pcmprevisto']}', '{$_POST['pcmrealizado']}')";
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

monta_titulo('PROCAMPO', '');

if( $_REQUEST['pcmid'] ){
	$sql = "SELECT
				*
			FROM
				academico.procampo
			WHERE
				pcmid = {$_REQUEST['pcmid']}";
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
	<input type="hidden" value="<?=$pcmid ?>" name="pcmid" id="pcmid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td valign="top" class="SubtituloEsquerda" width="20%" >
				Campus: 
			</td>
			<td>
				<?php 
					switch ( $_SESSION['academico']['orgid'] ){
						case '1':
							$funid = ACA_ID_CAMPUS;
						break;
						case '2':
							$funid = ACA_ID_UNED;
						break;
					}
					
						
					if($funid) {
						$boq = 'S';
						if( $pcmid ){
							$sql = "SELECT 
										e.entid as codigo,
										e.entnome as descricao
									FROM
										entidade.entidade e2
									INNER JOIN entidade.entidade e ON e2.entid = e.entid
									INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
									INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
									LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid
									LEFT  JOIN territorios.municipio mun ON mun.muncod = ed.muncod
									WHERE
										ea.entid = {$_SESSION['academico']['entid']} AND
										e.entstatus = 'A' 
										AND ef.funid = {$funid} AND
										e.entid = {$entidcampus} 
									ORDER BY
										e.entnome";
							$nome = $db->pegaLinha($sql);
						}else{
							$sql = "SELECT 
										e.entid as codigo,
										e.entnome as descricao
									FROM
										entidade.entidade e2
									INNER JOIN entidade.entidade e ON e2.entid = e.entid
									INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
									INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
									LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid
									LEFT  JOIN territorios.municipio mun ON mun.muncod = ed.muncod
									WHERE
										ea.entid = {$_SESSION['academico']['entid']} 
										AND	e.entstatus = 'A' 
										AND ef.funid = {$funid} AND
										e.entid NOT IN (SELECT entidcampus FROM academico.procampo )
									ORDER BY
										e.entnome";
						}
					} else {
						$sql = array();
					}
				//ver($esmid);
					if( $pcmid ){
						echo $nome['descricao'].'<input type="hidden" name="entidcampus" value="'.$nome['codigo'].'"/>';
					}else{
		                $db->monta_combo('entidcampus',$sql,$boq,'Selecione',$acao,$opc,'','100','S', 'entidcampus', $return = false,$entidcampus,$title= null);
					}
            	?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				N° de Vagas:
			</td>
			<td>
			<?=campo_texto('pcmnumvagas','S',"S",'Modelo',10,20,'','', '', '', '', 'id="pcmnumvagas"');?>
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
				<?=campo_texto('pcmprevisto','N',"$habilita",'Modelo',10,20,'####.#','', '', '', '', 'id="pcmprevisto"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Realizado
			</td>
			<td>
				<?=campo_texto('pcmrealizado','N',"$habilita1",'Modelo',10,20,'####.#','', '', '', '', 'id="pcmrealizado"');?>
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