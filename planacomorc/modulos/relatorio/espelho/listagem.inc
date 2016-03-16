<?php
include_once '_funcoesacoes.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$caminho_atual = $_SERVER ['REQUEST_URI'];

if ($_GET ['requisicao'] == 'excluir') {
	$sql = "DELETE FROM planacomorc.anexogeral  WHERE arqid = {$_GET['arqid']}";
	if ($db->executar ( $sql )) {
		// $sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
		// $db->executar($sql);
		$db->commit ();
	}
}
if ($_REQUEST ['download'] == 'S' && $_REQUEST ['arqid']) {
	$file = new FilesSimec ();
	$arqid = $_REQUEST ['arqid'];
	ob_clean ();
        
	$arquivo = $file->getDownloadArquivo ( $arqid );
	exit ();
}

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset ( $_REQUEST ['exercicio'] )) {
	$_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="col-lg-12">
    <div class="page-header">
        <h4 id="forms"><?php echo "Espelho das Ações - Listagem de Arquivos - " . $_SESSION ['exercicio']; ?></h4>
    </div>
    <br />

    <?php
    $sql = "SELECT
        con.arqid,
/*    '<a href=\"planacomorc.php?modulo=relatorio/espelho/cadastro&acao=A&arqid='|| con.arqid ||'&nomeArquivo='||con.angdsc||'&tipo='||con.angtip||'\"><img src=../imagens/anexo.gif title=\"Alterar\"></a>&nbsp;<img style=\"cursor:pointer; position:relative; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid ||');\">' as acoes, */
    '<a href=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
    con.angdsc,
    CASE angtip
      WHEN 'GE' THEN 'Programa de Gestão' 
      WHEN 'SU' THEN 'Programa Educação Superior'  
      WHEN 'BA' THEN 'Educação Básica' 
      WHEN 'TE' THEN 'Programa de Educação Tecnológica' 
    END as tipo_arquivo, 
    arq.arqtamanho,
    to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
    usu.usunome
    FROM
    planacomorc.anexogeral con
    INNER JOIN public.arquivo arq on con.arqid = arq.arqid
    INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
    WHERE angtipoanexo = 'E'
    AND angano = '".$_SESSION ['exercicio']."'
    order by angtip
    ";
    // dbg($sql,1);
//    $sql = sprintf ( $sql, $capid );
//    $cabecalho = array (
//                    "Alterar/Excluir",
//                    "Download",
//                    "Descrição",
//                    "Tipo",
//                    "Tamanho(bytes)",
//                    "Data inclusão",
//                    "Responsável" 
//    );
//    $db->monta_lista ( $sql, $cabecalho, 50, 10, 'N', '', '' );
    
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array (
        "Download",
        "Descrição",
        "Tipo",
        "Tamanho(bytes)",
        "Data inclusão",
        "Responsável" 
    ));
    $listagem->setQuery($sql);
    $listagem->setAcoes(array(
        'edit' => 'editLimite',
        'delete' => 'deleteLimite'
    ));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>
    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>
    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-primary" id="voltar" type="button" onclick="voltar();">Voltar</button>
        </div>
    </div>
</div>

<script>
    function voltar(){
        window.location = 'planacomorc.php?modulo=relatorio/espelho/espelhoacoes&acao=A';
    }
    
    function editLimite(arqid) {
        window.location = "planacomorc.php?modulo=relatorio/espelho/cadastro&acao=A&arqid="+arqid;
    }
    
    function deleteLimite(arqid){
        if(confirm("Deseja realmente excluir este arquivo?")){
            window.location += '&requisicao=excluir&arqid='+arqid;
        }
    }
</script>