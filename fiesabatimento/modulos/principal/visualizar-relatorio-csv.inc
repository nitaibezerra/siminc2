<?php
    require (APPRAIZ . 'includes/library/simec/Listagem.php');
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
    

    if ($_GET ['requisicao'] == 'excluir') {
        $sql = "TRUNCATE table fiesabatimento.relatorio_validacao_docente;";
        if ($db->executar($sql)) {
            $db->commit ();
        }
        echo "<script>window.location = 'fiesabatimento.php?modulo=principal/visualizar-relatorio-csv&acao=A';</script>";
        die;
    }
?>

<div class="col-lg-12">
    <div class="page-header">
        <h4 id="forms">Relatório.</h4>
    </div>
    <br />
    
    <?php 
        $sql = "
            SELECT 
                nu_cpf,
                nu_quantidade_acesso,
                nu_validacao_pendente || 'º Validação',
                ds_validacao_pendente,
                CASE WHEN st_validacao = 'S' THEN 'Sim' ELSE 'Não' END,
                dt_validacao
            FROM 
                fiesabatimento.relatorio_validacao_docente
        ;";

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(array (
            "CPF",
            "Quantidade de acessos",
            "Etapa de validação",
            "Menssagem de erro",
            "Validado",
            "Data de validação"
            ));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>
    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>
    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-danger" id="btnApagar" type="button">Apagar Relatório</button>
            <button class="btn btn-primary" id="btnVoltar" type="button">Voltar</button>
        </div>
    </div>
    
</div>

<script>
    jQuery(document).ready(function(){
        
        jQuery('#btnVoltar').click(function(){
            voltar();
        });
        
        jQuery('#btnApagar').click(function(){
            apagarRelatorio();
        });
    });
    
    function voltar(){
        window.location = 'fiesabatimento.php?modulo=principal/importar-csv&acao=A';
    }
    
    function apagarRelatorio(arqid) {
        if(confirm("Deseja realmente apagar o relatório?")){
            window.location = "fiesabatimento.php?modulo=principal/visualizar-relatorio-csv&acao=A&requisicao=excluir";
        }
    }
</script>