<?PHP
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$perfil = pegarPerfil($_SESSION['usucpf']);

if($_GET['download'] == 'S'){
	
	$sql = "SELECT tvgdsc
		    FROM cap.termoviagem
		    WHERE tvgid = ".$_REQUEST['tvgid'];
	$html = $db->pegaUm($sql);

	//dbg($htmlTermo,1);
	/*
	ob_clean();
	echo $htmlTermo;
	$html = ob_get_contents();
	ob_clean();
	*/
	$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));

	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename='cap_termo_".date('dmY').".pdf");
	echo $contents;
	exit;
}

if($_SESSION['cap']['afpid']){
	$docid = criarDocumento($_SESSION['cap']['afpid']);
} 

$esdid = pegarEstadoDocumento($docid);

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil)){
	$modulo = "Módulo Administrativo";
} else {
	$modulo = "Módulo Servidor";
}

$titulo_modulo = "Afastamento do País";
monta_titulo( $titulo_modulo, $modulo);

cabecalhoCAP();
?>

<br>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	function excluirDocAnexo(arqid){
		if(confirm("Deseja realmente excluir o Arquivo?")){
			$.ajax({
			url: 'cap.php?modulo=principal/documentos&acao=A',
			data: { requisicao: 'excluir', arquivo: arqid},
			async: false,
			success: function(data) {
					alert('Arquivo excluído com sucesso!');
					$("#div_documento").html(data);
		    	}
			});
		}
	}

	function validarDocumento(){
		if($('[name=arquivo]').val() == ''){
			alert('O campo "Arquivo" é obrigatório!');
			$('[name=arquivo]').focus();
			return false;
		}
		if($('[name=tpdid]').val() == ''){
			alert('O campo "Tipo de Documento" é obrigatório!');
			$('[name=tpdid]').focus();
			return false;
		}
		if($('[name=arqdescricao]').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			$('[name=arqdescricao]').focus();
			return false;
		}		
		return true;		
	}
</script>

<?PHP if(!empty($_SESSION['cap']['afpid'])){ ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr bgcolor="#CCCCCC">
		<td><b>Servidor:</b>&nbsp;<?PHP echo exibirNomeServidor($_SESSION['cap']['afpid']); ?></td>
	</tr>
</table>
<br>
<?PHP } ?>

<?PHP 
$arMnuid = array();
$db->cria_aba( ABA_CAD_AFASTAMENTO, $url, '', $arMnuid );
?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center" colspan="2">
        Formulário de Autorização de Afastamento do País - MISSÃO OFICIAL<br>
        Prazo limite para a entrega da solicitação: 15 dias antes do ínicio da missão) 
    	</td>
    </tr>
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2"><b>Termos</b></td>
    </tr>    
 </table>

<br>

<div id="div_termos">
<?PHP
	
	$sql = "SELECT 		'<a href=\"cap.php?modulo=principal/termos&acao=A&download=S&tvgid='|| tvgid ||'\" ><center><img src=\"../imagens/anexo.gif\" border=\"0\"></center></a>' as acao,
						CASE WHEN tvgtipo=1 THEN '<center>AUTORIZADA</center>'
				            WHEN tvgtipo=2 THEN '<center>ALTERADA</center>'
				            WHEN tvgtipo=3 THEN '<center>SEM EFEITO</center>'
				        END as tipo,
				        '<center>'||to_char(tvgdtinclusao::TIMESTAMP,'DD/MM/YYYY HH24:MI')||'</center>' as dtinclusao
		    FROM 		cap.termoviagem
		    WHERE tvgstatus = 'A'
		    AND afpid = ".$_SESSION['cap']['afpid']."
		   	ORDER BY 	tvgdtinclusao DESC";
	$cabecalho = array("Ação","Situação","Data do Termo");					
	$db->monta_lista_simples($sql, $cabecalho, '50', '10', '', '', '', ''); 
	
?>
</div>
