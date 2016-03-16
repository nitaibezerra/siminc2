<?php
$sql = <<<DML
    SELECT
        con.arqid,
        arq.arqnome as arquivo,
        arq.arqextensao,
        arq.arqid as id,
        con.angdsc,
        CASE arptipo
            WHEN 'GE' THEN 'Programa de Gestão'
            WHEN 'SU' THEN 'Programa Educação Superior'
            WHEN 'BA' THEN 'Educação Básica'
            WHEN 'TE' THEN 'Programa de Educação Tecnológica'
            WHEN 'OU' THEN 'Outras Ações'
        END as tipo_arquivo,
        arq.arqtamanho,
        to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
        usu.usunome
    FROM spo.anexogeral con
    INNER JOIN public.arquivo arq on con.arqid = arq.arqid
    INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
    WHERE angtipoanexo = 'E'
        AND con.exercicio = '{$_SESSION ['exercicio']}'
    order by arptipo
DML;
  
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array (
        "Download",
        "Descrição",
        "Tipo",
        "Tamanho(bytes)",
        "Data inclusão",
        "Responsável"
    ));
    $listagem->esconderColunas(array('arqextensao','id'));
    $listagem->addCallbackDeCampo('arquivo', function($arquivo,$linha){
        $retorno = <<<HTML
        <a href="" class="buttonDownload" title="Download" data-request="&requisicao=download&arqid={$linha['id']}">
            {$arquivo}.{$linha['arqextensao']}
        </a>
HTML;
        return $retorno;
    });
    $listagem->setQuery($sql);
    if($permissaoAcesso){
        $listagem->setAcoes(array(
            'edit' => 'editar',
            'delete' => 'deletar'
        ));
    }else{
        $listagem->esconderColunas('arqid');
    }
    $listagem->turnOnPesquisator();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>


<script>
    function deleteLimite(arqid){
        if(confirm("Deseja realmente excluir este arquivo?")){
            window.location += '&requisicao=excluir&arqid='+arqid;
        }
    }
</script>