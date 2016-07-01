<?php
// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST ['exercicio'])) {
    $_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";
include APPRAIZ . "includes/cabecalho.inc";
?>
<style>
    #divAcoes{background-color:yellowgreen}
    #divSubacoes{background-color:#00CED1}
    #divCap{background-color:#FF6347}
    #divManuais{background-color:#EEB422}
    #divPTRES{background-color:darksalmon}
    #divPrevRecOrc{background-color:#CC6666}
    #divTabelaApoio{background-color:#18bc9c}
    #divComunicados{background-color:royalblue}
</style>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" language="JavaScript">
$(document).ready(function() {
    inicio();
});
function abrirArquivo(arqid){
    window.location = 'recorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid='+arqid;
}
</script>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0"
       cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divCap" class="divCap">
                <span class="tituloCaixa">Alteração de Receita</span>
                <?php
                    $params['texto'] = 'Acompanhamento';
                    $params['tipo'] = 'acompanhamento';
                    $params['url'] = 'recorc.php?modulo=principal/captacao/acompanhamento&acao=A';
                    montaBotaoInicio($params);

                    $params['texto'] = 'Alterar Previsão';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'recorc.php?modulo=principal/captacao/listar&acao=A';
                    montaBotaoInicio($params);
                ?>
                 <span class="subTituloCaixa">Relatórios</span>
                <?php
                    $params['texto'] = 'Extrato de Alterações de Previsões';
                    $params['tipo'] = 'relatorio';
                    $params['url'] = 'recorc.php?modulo=principal/captacao/extrato&acao=A';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div id="divTabelaApoio" class="divCap" data-request="">
                <span class="tituloCaixa">Parâmetros SPO</span>
                <?php
                    $params['texto'] = 'Vínculos de previsão por período';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A';
                    montaBotaoInicio($params);

                    $params['texto'] = 'Período de Referência';
                    $params['tipo'] = 'calendario';
                    $params['url'] = 'recorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'Upload Previsão';
                    $params['tipo'] = 'upload';
                    $params['url'] = 'recorc.php?modulo=principal/captacao/previsaoupload&acao=A';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                    $params = array();
                    $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                    $params['tipo'] = 'snapshot';
                    $params['url'] = 'recorc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php 
                    $params['texto'] = 'Manual de Preenchimento (Alteração de Receita)';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'manual_preenchimento_captacao.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'EMENTÁRIO DE CLASSIFICAÇÃO DAS RECEITAS ORÇAMENTÁRIAS DA UNIÃO 2015';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'ementario_2015.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'EMENTÁRIO - 2014 - ANEXO I - Natureza-Fonte Portaria SOF nº 9/2001';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'ementario_2014_anexo_1.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'EMENTÁRIO - 2014 - ANEXO II -  Receitas da Seguridade';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'ementario_2014_anexo_2.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'EMENTÁRIO - 2014 - ANEXO III - Natureza_União Estados Mun. Portaria Conjunta SOF_STN nº 163_2001';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'ementario_2014_anexo_3.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'EMENTÁRIO - 2014 - ANEXO IV - Portaria SOF Nº 01, 19-02-2001 - Fontes de Recursos';
                    $params['tipo'] = 'pdf';
                    $params['url'] = 'ementario_2014_anexo_4.pdf';
                    $params['target'] = '_blank';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div id="divComunicados" class="divCap">
                <span class="tituloCaixa">Comunicados</span>
                <?php
                montaComunicados();
                ?>
            </div>
            
        </td>
    </tr>
</table>