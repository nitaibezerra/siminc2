<?php

if(!$_REQUEST['mcpid'] || !$_SESSION['emiid']) {
	echo "<script>
			alert('Variável não encontrada. Refaça o procedimento.');
			window.close()';
		  </script>";
	exit;
}

//var_dump($_REQUEST);
if($_REQUEST['requisicao'] == 'salvaProfissionais'){
		
	if(!empty($_REQUEST['num_prof_'.$_REQUEST['mcpid']]) || 
	   !empty($_REQUEST['num_equipe_'.$_REQUEST['mcpid']])  || 
	   !empty($_REQUEST['num_outros_'.$_REQUEST['mcpid']])){
	   	
	   	$numProf   = $_REQUEST['num_prof_'.$_REQUEST['mcpid']] ? str_replace('.','',$_REQUEST['num_prof_'.$_REQUEST['mcpid']]) : 'null'; 
		$numEquipe = $_REQUEST['num_equipe_'.$_REQUEST['mcpid']] ? str_replace('.','',$_REQUEST['num_equipe_'.$_REQUEST['mcpid']]) : 'null'; 
		$numOutros = $_REQUEST['num_outros_'.$_REQUEST['mcpid']] ? str_replace('.','',$_REQUEST['num_outros_'.$_REQUEST['mcpid']]) : 'null'; 
		$mcpid     = $_REQUEST['mcpid'];
		$emiid     = $_SESSION['emiid'];
		
		$sql = 'INSERT INTO em.emiprofissionalenvolvido 
					(
						preqtdprofessor,
						preqtddirecao,
						preqtdoutros,
						mcpid,
						emiid
					)
				VALUES
					(
						'.$numProf.', 
						'.$numEquipe.', 
						'.$numOutros.', 
						'.$mcpid.', '.$emiid.'
					);';
	}
	
	if($sql){			
		$sql = " DELETE FROM em.emiprofissionalenvolvido WHERE emiid = {$_SESSION['emiid']} AND mcpid = {$_REQUEST['mcpid']}; ".$sql;		
		$db->executar($sql);
		$db->commit();		
		header("Location: em.php?modulo=principal/profissionaisEducacao&acao=A&emiid={$_SESSION['emiid']}&mcpid={$_REQUEST['mcpid']}");
	}
}

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid']   );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}
//var_dump($_SESSION['emiid']);
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>	    	    
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script>
	    	function salvarProfissionais(mcpid)
			{				
				document.formulario.submit();
			}
	    </script>
	</head>
	<body>
		<?php
		monta_titulo("Profissionais de Educação", "PRC 2012");
		echo montaCabecalhoPRC($_SESSION['emiid'], $_SESSION['mcpid'], $_REQUEST['papid'], false);
		//echo "<br/>";		
		?>
		<form name="formulario" method="post">
			<input type="hidden" name="requisicao" value="salvaProfissionais" />
			<input type="hidden" name="mcpid" value="<?php echo $_REQUEST['mcpid'] ?>" />
			<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="2" cellPadding="2" >
				<tr>
					<td style="font-weight:bold" bgcolor="#DCDCDC" align="center" colspan="5">Número de Profissionais Envolvidos</td>
				</tr>
				<tr bgcolor="#e9e9e9" >
					<td style="font-weight:bold" align="center" >Professor(a)</td>
					<td style="font-weight:bold" align="center" >Equipe Direção</td>
					<td style="font-weight:bold" align="center" >Outros Profissionais</td>
					<td style="font-weight:bold" align="center" colspan="2" >Total</td>
				</tr>
				<?php
				$sql = "select * from em.emiprofissionalenvolvido WHERE mcpid = {$_REQUEST['mcpid']} and emiid = {$_SESSION['emiid']}";				
				$arrProf = $db->pegaLinha( $sql );
				$mcpid = $_REQUEST['mcpid'];
				?>
				<tr>
					<td align="center" ><?php $num_prof_{$mcpid}   = number_format($arrProf['preqtdprofessor'],2,'','.'); echo campo_texto("num_prof_{$mcpid}","S",$ativo,"",10,20,"###","","right","","","","calculaTotalProfissionais($mcpid)",$num_prof_{$mcpid}) ?></td>
					<td align="center" ><?php $num_equipe_{$mcpid} = number_format($arrProf['preqtddirecao'],2,'','.');   echo campo_texto("num_equipe_{$mcpid}","S",$ativo,"",10,20,"###","","right","","","","calculaTotalProfissionais($mcpid)",$num_equipe_{$mcpid}) ?></td>
					<td align="center" ><?php $num_outros_{$mcpid} = number_format($arrProf['preqtdoutros'],2,'','.');    echo campo_texto("num_outros_{$mcpid}","S",$ativo,"",10,20,"###","","right","","","","calculaTotalProfissionais($mcpid)",$num_outros_{$mcpid}) ?></td>
					<td align="center" colspan="2" id="td_total_profissionais_<?php echo $mcpid ?>" ><?php echo $arrProf ? number_format($arrProf['preqtdprofessor'] + $arrProf['preqtddirecao'] + $arrProf['preqtdoutros'],2,'','.') : 0 ?></td>
				</tr>
				<tr bgcolor="#e9e9e9" >
					<td align="center" colspan="5" >
						<input type="button" value="Salvar" name="btn_salvar" class="btn_salvar" onclick="salvarProfissionais('<?php echo $_REQUEST['mcpid'] ?>')" <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?>/>
						&nbsp;
						<input type="button" value="Fechar" onclick="window.close();" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>