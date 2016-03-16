<?php 
include_once '_funcoes_maismedicos.php';

if( !$_SESSION['par']['rqmid'] ){
	echo "<script>alert('Erro de sessão');window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';</script>";
	die();
}

if($_REQUEST['requisicao'] == 'salvarParecer'){
    salvarParecer($_POST);
    die();
}

$parecer = recuperaDadosParecer($_SESSION['par']['rqmid']);
$parecer = is_array($parecer) ? $parecer : Array();
extract($parecer);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	function imprimir(){
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosParecerImprimir&acao=A', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
	}	
</script>

<form action="par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Parecer" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
	<input type="hidden" id="requisicao" name="requisicao" value="salvarParecer"/>
	<input type="hidden" id="rqmid" name="rqmid" value="<?php echo $_SESSION['par']['rqmid']; ?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
	    	<td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Parecer Técnico</b></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="25%">1. Dados do município:</td>
	        <td><?php echo str_replace("-","/",recuperaMunicipioEstado()); ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">2. Introdução:</td>
	        <td><?php echo campo_textarea('rqmparintroducao', 'N', 'S', '', '150', '20', '8000','','','','','',$rqmparintroducao); ?></td>
	    </tr>		
	    <tr>
	        <td class="SubTituloDireita">3. Análise:</td>
	        <td><?php echo campo_textarea('rqmparanalise', 'N', 'S', '', '150', '20', '8000','','','','','',$rqmparanalise); ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">4. Conclusão:</td>
			<td><?php echo campo_textarea('rqmparconclusao', 'N', 'S', '', '150', '20', '8000','','','','','',$rqmparconclusao); ?></td>
	    </tr>		
		<tr>
                    <td align="center" bgcolor="#CCCCCC" colspan="2">
                        <?  if( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ){ ?>
                                <input type="submit" value="Salvar" id="btnSalvar"/>
                        <?  }else{ ?>
                                <input type="button" value="Salvar" id="salvar" disabled="disabled"/>
                        <?  } 
                            if(!empty($rqmparintroducao) && !empty($rqmparanalise) && !empty($rqmparconclusao)){ 
                        ?>
                                <input type="button"" value="Imprimir" id="btnImprimir"  onclick="imprimir();";/>
                        <? } ?>
                    </td>
		</tr>    
	</table>
</form>
