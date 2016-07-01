<?php

include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

unset($_SESSION['academico']['boPerfilSuperUsuario'],$_SESSION['academico']['boPodeCadastrar']);

# verifica se tem permissão de gravar
$habilitado = (possuiPerfilCadastro()) ? 'S' : 'N'; 

if( possuiPerfil(array(PERFIL_SUPERUSUARIO, PERFIL_MECCADASTRO) ) ){
	$_SESSION['academico']['boPerfilSuperUsuario'] = true;
} elseif( possuiPerfil(array(PERFIL_IFESCADBOLSAS, PERFIL_IFESCADCURSOS, PERFIL_IFESCADASTRO) ) ){
	$_SESSION['academico']['boPodeCadastrar'] = true;
}

if( $_SESSION['academico']['boPodeCadastrar'] || $_SESSION['academico']['boPerfilSuperUsuario'] ){
	// campos habilitados
	$acao = "'<center><a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,";
	$desabilitado = "";
	$habil = "S";
}else{
	// campos desabilitados
	$acao = "'<center><a style=\" cursor: pointer;\"><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></a></center>' as acao,";
	$desabilitado = " disabled='disabled'";
	$habil = "N";
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'academico.php?modulo=principal/anexoTermoCooperacao&acao=A';</script>";
    exit;
}

if($_FILES["arquivo"]){
	if( $_SESSION['academico']['tmcid'] )
	{
		$campos	= array("axtdtinclusao" 	=> "now()",
						"axtdesc"     		=> "'".$_POST['axtdesc']."'",
						"tpaid"     		=> $_POST['tpaid'],
						"axtstatus" 		=> "'A'",
						"tmcid"				=> $_SESSION['academico']['tmcid']
					   );	
		
		$file = new FilesSimec("anexostermocooperacao", $campos ,"academico");
		
		$arquivoSalvo = $file->setUpload( $_POST['axtdesc']);
		
		if($arquivoSalvo){
			?>
			<script type="text/javascript"> 
				alert("Operação realizada com sucesso!");
				location.href = 'academico.php?modulo=principal/anexoTermoCooperacao&acao=A';
			</script>
			<?php	
			exit;
		}
	}
	else 
	{
		?>
		<script type="text/javascript"> 
			alert("Ocorreu um erro. Contate o administrador do sistema.");
			location.href = 'academico.php?modulo=principal/anexoTermoCooperacao&acao=A';
		</script>
		<?php	
		exit;
	}
}
 
if($_REQUEST['arqidDel'] != ''){
    $anexos = array();
    $sql = "UPDATE academico.anexostermocooperacao SET axtstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'academico.php?modulo=principal/anexoTermoCooperacao&acao=A';</script>";
}
 
require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, array() );
monta_titulo( $titulo_modulo, '' );
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function() {
	
	$('#btnSalvar').click(function(){

		var msg = "";
		
		if($('#arquivo').val() == ''){
			msg += "O campo Arquivo é obrigatório.\n";
		}
		if($('#tpaid').val() == ''){
			msg += "O campo Tipo é obrigatório.\n";
		}
		if($('#anxdesc').val() == ''){
			msg += "O campo Descrição é obrigatório.\n";
		}
				
		if(msg){
			alert(msg);
			return false;
		}
		
		$('#formulario').submit();	
	});

});

function excluirAnexo( arqid ){
	if ( confirm( 'Deseja excluir o Documento?' ) ) {
		location.href= window.location+'&arqidDel='+arqid;
	}
}

//-->
</script>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0"> 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
<tr>
			<td></td><td></td>
			<td rowspan="6">
				<?php 
				$tmcid = $_SESSION['academico']['tmcid'];
				if( $tmcid )
				{
					$docid = tcPegarDocid($tmcid);
					
					if($docid)
					{ 
						wf_desenhaBarraNavegacao( $docid , array( 'tmcid' => $tmcid ) );
					}				
				}
				?>
			</td>
		</tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
        <td width='50%' colspan="2">
            <input type="file" name="arquivo" id="arquivo"<?php echo $desabilitado; ?>/>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>      
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
          $sql = "SELECT 
          				tpaid AS codigo, 
          				tpadsc AS descricao
                  FROM 
                  		academico.tipoarquivo where tpastatus = 'A' order by tpadsc"; 
        $db->monta_combo('tpaid', $sql, $habil, "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
        ?></td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td><?= campo_textarea( 'axtdesc', 'S', $habil, '', 60, 2, 250 ); ?></td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
        <td height="30" colspan="2"><input type="button" id="btnSalvar" value="Salvar" <?php echo $desabilitado; ?>  /></td>
    </tr> 
</table>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?php 
    	
        $sql = "SELECT
                        {$acao}                       
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        tpa.tpadsc,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/anexoTermoCooperacao&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho, 
                        arq.arqdescricao
                        --arq.arqdescricao                            
                        --usu.usunome
                    FROM
                        ((public.arquivo arq INNER JOIN academico.anexostermocooperacao a
                        ON arq.arqid = a.arqid) INNER JOIN academico.tipoarquivo tpa
                        ON tpa.tpaid = a.tpaid) 
                    WHERE
                        arq.arqstatus = 'A'
                        AND a.tmcid = ".$_SESSION['academico']['tmcid']."";
		
        $cabecalho = array( "Ação",
                            "Data Inclusão",
                            "Tipo Arquivo",
                            "Nome Arquivo",
                            "Tamanho (Mb)",
                            "Descrição Arquivo"
                            //"Responsável"
                            );
        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
    ?>
</table>
</form>