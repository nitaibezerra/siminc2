<?
pegaSessoes(1);
$copid	= $_REQUEST['copid'];
$copidinciso = $_REQUEST['copidinciso'];
$prtid = $_SESSION["academico"]["prtid"];
if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '') ){	
	$sql = "SELECT COUNT(copordem)				
			FROM academico.composicaoportaria cp INNER JOIN  academico.tipocomposicao tp ON cp.tpcid = tp.tpcid 
			WHERE tp.tpcid IN (1,4) AND cp.prtid = ".$prtid;					
	$copordem = $db->pegaUm($sql);
	if($dados){
		$copordem = $copordem+1;	
	} 
	else{
		$copordem = 0;
	}
	
	switch($_REQUEST[evento]) {
		
		case 'I':	
			$tpcid		= $_REQUEST['tpcid']; 
			$copdesc	= $_REQUEST['copdesc']; 
			if (isset($copidinciso)){
				$copordem = 9999;
			}else{
				$copidinciso=0;				
			}
			if ($copidinciso==0){
				$copidinciso='NULL';
			}
			
			if($copid){						
				$sql = "
				SELECT 
					copid				
				FROM  academico.composicaoportaria 
				WHERE 
				copid = ".$copid;					
				$dados = $db->pegaUm($sql);
			}
			
			// caso não exista o indicador insere um novo
			if(!$dados){	
				$sql_I = "INSERT 
						  INTO academico.composicaoportaria 
							  (prtid, tpcid, copdesc,copordem,copidinciso)
						  VALUES 
						  	   (".$prtid.", ".$tpcid.", '".$copdesc."', ".$copordem.", ".$copidinciso.") ";	
				//dbg($sql_I,1);

				$db->executar($sql_I);
			//caso exista o indicador altera os dados do mesmo	
			}else{
				
				$sql_U = "
					UPDATE academico.composicaoportaria 
					SET
						prtid		= ".$prtid.",
						tpcid		= ".$tpcid.",
						copdesc		= '".$copdesc."'					
					WHERE copid 	= ".$copid;
				
				$db->executar($sql_U);				
			}
			$db->commit();
			echo '<script>
					function salvar(){
						alert("Operação realizada com sucesso!");
						window.opener.location.reload(); 
						window.close();
					}
					salvar();
			  </script>';
		exit;
		break;

		case 'E':
			$sql = "
			SELECT 
				copid				
			FROM  academico.composicaoportaria 
			WHERE 
			copidinciso = ".$copid;					
			$dados = $db->carregar($sql);
			
			if (!$dados){
				$copid = $_REQUEST['copid']; 	
				$sql_D = "DELETE FROM academico.composicaoportaria WHERE copid = $copid";	
				//dbg($sql_D,1);
				$db->executar($sql_D);
				$db->commit();
				echo "<script>window.opener.location.reload();</script>";
			} else {
				
				echo "<script> alert('Não é possível excluir este Artigo pois ele possui incisos ou parágrafos vinculados.'); </script>";				
			}
		exit;
		break;	
		
	}
	
}

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   //PERFIL_IFESCADCURSOS, 
					   //PERFIL_IFESCADASTRO, 
					   PERFIL_IFESCADTCU,
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

if($copid){						
		$sql = "
			SELECT copid, tpcid, copdesc, copordem				
			FROM  academico.composicaoportaria 
			WHERE copid = ".$copid;					
		$dados = $db->pegaLinha($sql);
		$copdesc=$dados["copdesc"];
		$tpcid=$dados["tpcid"];
		$copordem=$dados["copordem"];
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
<!--

//-->
</script>	
<form name="form_portarias" id="form_portarias" >
<input type="hidden" name="copidinciso" id="copidinciso" value='<?=$copidinciso;?>'>
<input type="hidden" name="copid" id="copid" value='<?=$_REQUEST['copid'];?>'>
<input type="hidden" name="prtid" id="prtid" value='<?=$_REQUEST['prtid'];?>'>
<input type="hidden" name="copordem" id="copordem" value='<?if($copid){	echo $copordem;}?>'>

<table width="90%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela">
		<tr>
            <td >
            <strong>Tipo</strong></td>
            <td>
			<?	
				if ($copidinciso){
				$sql = "SELECT 
						tpcid AS codigo, 
						tpcdesc AS descricao 
						FROM academico.tipocomposicao 
						WHERE tpcid NOT IN (1,4)
						ORDER BY tpcdesc";
				}
				else{
					$sql = "SELECT 
						tpcid AS codigo, 
						tpcdesc AS descricao 
						FROM academico.tipocomposicao
						WHERE tpcid IN (1,4) 
						ORDER BY tpcdesc";
					
				}
				//dbg($sql,1);
				echo $db->monta_combo('tpcid',$sql,'S','Selecione...','','','','100%','S', 'tpcid');
				?>
            </td>			
		</tr>
	<tr>
            <td>
            <strong>Conteúdo</strong></td>
	<td>
		<? 
		
		echo campo_textarea('copdesc','N', $habil, '', 100, 10, 1000 );?>
		</td>
	</tr>
	<?php if( $permissoes['gravar'] ){?>
	<tr><td></td><td><input type="button" class="botao" name="btassociar" value="Gravar" onclick="gravar();" ></td></tr>
	<?php } ?>
</table>
<input type="hidden" name="evento" value="I">
</form>
<script type="text/javascript">
	function gravar(){
		copid=document.getElementById("copid").value;
		tpcid=document.getElementById("tpcid").value;
		copdesc=document.getElementById("copdesc").value;
		prtid=document.getElementById("prtid").value;
		copordem=document.getElementById("copordem").value;
		if (tpcid !="")
			window.location.href = 'academico.php?modulo=principal/cadpartes_portaria&acao=C&copidinciso=<?=$copidinciso;?>&tpcid='+tpcid+'&prtid='+prtid+'&copdesc='+copdesc+'&copid='+copid+'&evento=I&copordem='+copordem,'_top';
		else
			alert("O Campo Tipo é Obrigatório");
		//ndow.opener.location.reload();		
	}
</script>
