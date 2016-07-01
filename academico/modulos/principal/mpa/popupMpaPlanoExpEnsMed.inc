<?php
$perfis = pegaPerfilGeral();
$habilita = 'S';
if(in_array(PERFIL_ADMINISTRADOR,$perfis)||$db->testa_superuser()){
	$habilita = 'N';
};
function salvar(){
     
    global $db;
    
    if( $_REQUEST['esmid'] ){
    	$sql = "UPDATE academico.expensinomedico SET
    				esmtipo = '".$_POST['esmtipo']."', 
    				esmnumvagas = ".$_POST['esmnumvagas'].", 
    				esmprevisto = '".$_POST['esmprevisto']."', 
    				esmrealizado = '".$_POST['esmrealizado']."'
    			WHERE
    				esmid = {$_REQUEST['esmid']}";
    }else{
		$sql = "INSERT INTO academico.expensinomedico (entidunidade, entidcampus, esmtipo, esmnumvagas,  esmprevisto, esmrealizado) 
				VALUES('{$_SESSION['academico']['entid']}', '".$_POST['entidcampus']."', '".$_POST['esmtipo']."',".$_POST['esmnumvagas'].",'".$_POST['esmprevisto']."','".$_POST['esmrealizado']."' )";
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

monta_titulo('Plano de Expansão do Ensino Médico', '');

if( $_REQUEST['esmid'] ){
	$sql = "SELECT
				*
			FROM
				academico.expensinomedico
			WHERE
				esmid = {$_REQUEST['esmid']}";
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
	<input type="hidden" value="<?=$esmid ?>" name="esmid" id="esmid" />
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
						if( $esmid ){
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
										ea.entid = {$_SESSION['academico']['entid']} AND
										e.entstatus = 'A' 
										AND ef.funid = {$funid} AND
										e.entid NOT IN (SELECT entidcampus FROM academico.expensinomedico )
									ORDER BY
										e.entnome";
						}
					} else {
						$sql = array();
					}
				
					if( $esmid ){
						echo $nome['descricao'].'<input type="hidden" name="entidcampus" value="'.$nome['codigo'].'"/>';
					}else{
		                $db->monta_combo('entidcampus',$sql,$boq,'Selecione',$acao,$opc,'','100','S', 'entidcampus', $return = false,$entidcampus,$title= null);
					}
            	?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Tipo:
			</td>
			<td>
				<?php 
					$sql = Array(Array('codigo'=>'N','descricao'=>'Curso Novo'),
								 Array('codigo'=>'E','descricao'=>'Expansão'));
				
		             $db->monta_combo('esmtipo',$sql,'S','Selecione',$acao,$opc,'','150','S', 'esmtipo', $return = false,$esmtipo,$title= null);
	             ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				N° de Vagas:
			</td>
			<td>
			<?=campo_texto('esmnumvagas','S',"S",'Modelo',10,20,'','', '', '', '', 'id="esmnumvagas"');?>
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
				<?=campo_texto('esmprevisto','N',"$habilita",'Modelo',6,6,'####.#','', '', '', '', 'id="esmprevisto"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Realizado
			</td>
			<td>
				<?=campo_texto('esmrealizado','N',"$habilita",'Modelo',6,6,'####.#','', '', '', '', 'id="esmrealizado"');?>
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