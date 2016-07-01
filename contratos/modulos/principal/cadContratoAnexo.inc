<?php 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();


$campos	= array("ctrid"				=> $_SESSION['ctrid'],
				"ancdatainclusao" 	=> "now()" ,
                                "usucpf"    		=> "'{$_SESSION['usucpf']}'",
				"tpaid"     		=> $_POST['tpaid'],
				"ancstatus" 		=> "'A'"
				);	
				
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'contratos.php?modulo=principal/cadContratoAnexo&acao=A';</script>";
    exit;
} 


$file = new FilesSimec("ctanexo", $campos ,"contratos");



if($_FILES["Arquivo"]){	
		 
	$arquivoSalvo = $file->setUpload( $_POST['arqdescricao']);
	
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'contratos.php?modulo=principal/cadContratoAnexo&acao=A';</script>";	
	}
}
 
if($_REQUEST['arqidDel'] != ''){
   
    $anexos = array();
    $sql = "UPDATE contratos.ctanexo SET ancstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'contratos.php?modulo=principal/cadContratoAnexo&acao=A';</script>";

}
// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,
// 		PERFIL_CONSULTA_UNIDADE,
// 		PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_EQUIPE_TECNICA_UNIDADE));
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );
montaCabecalhoContrato($_SESSION['ctrid']);

?>
<script src="../includes/prototype.js"></script>
<? if(!$desabilitado) : ?>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0">

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top;" width="25%">Arquivo:</td>
        <td>
            <input type="file" name="Arquivo" id="Arquivo"/>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>      
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
          $sql = "
            SELECT tpaid AS codigo, tpadescricao AS descricao
            FROM contratos.tipoanexo
			where tpastatus = 'A' 
			order by 2
        "; 
        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
        ?></td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
        <td height="30"><input type="button" name="botao" value="Salvar" onclick="validaForm();"></td>
    </tr> 
</table>
</form>
<? endif; ?>

    <?
        $sql = "SELECT
                        '<center>".(($desabilitado)?"":"<a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>")."</center>' as acao,                       
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        tarq.tpadescricao,
                        arq.arqdescricao,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho ,
                        arq.arqdescricao,                               
                        usu.usunome
                    FROM
                        ((public.arquivo arq INNER JOIN contratos.ctanexo anc
                        ON arq.arqid = anc.arqid) INNER JOIN contratos.tipoanexo tarq
                        ON tarq.tpaid = anc.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  anc.ctrid = " .$_SESSION['ctrid'];
		 
        $cabecalho = array( "Ação",
                            "Data Inclusão",
                            "Tipo Arquivo",
                            "Descrição Arquivo",
                            "Nome Arquivo",
                            "Tamanho (Mb)",    
                            "Responsável");
        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
    ?>

<script language="javascript" type="text/javascript">

 
var query;
var objForm = document.forms["formulario"];

function validaForm(){
	var msg = new Array();

	
	if( objForm.Arquivo.value == '' ){
		msg.push("Selecione um arquivo");
	}

	if( document.getElementById('tpaid').value == '' ){
		msg.push("O campo tipo é obrigatório");
	}
	
	if( objForm.arqdescricao.value == '' ){
		msg.push("O campo descrição é obrigatório");
	}

	if( msg.length > 0 ){
		alert( msg.join("\n") );
		return false;
	}else{
		objForm.submit();
	}
}
	 
function excluirAnexo( arqid ){
	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}
 
</script>