<?php

    require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    include_once APPRAIZ . "includes/funcoesspo_componentes.php";
    
    $cpf = $_SESSION['usucpf'];
    $listaPerfil = pegaPerfilGeral();
    $listaEscola = buscarEscolaUsuarioLogado($cpf, $listaPerfil);
    
    include APPRAIZ . "includes/cabecalho.inc";

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>

<div class="col-lg-12">
    <div align="right">
        <table class="listagem" border=1  align="right" cellspacing="3" cellpadding="3">
            <tr>
                <td align="center">
                    <b>Manuais para Download<b>
                </td>
            </tr>
            <tr>
                <td align="left">
                    <?php
                        echo '<a href="/eja/arquivos/manual_diretor.pdf" target="_blank" title="Clique para fazer o download">- MANUAL DIRETOR</a>';
                        echo '<br><a href="/eja/arquivos/manual_gestor_estadual.pdf" target="_blank" title="Clique para fazer o download">- MANUAL GESTOR ESTADUAL</a>';
                        echo '<br><a href="/eja/arquivos/manual_gestor_municipal.pdf" target="_blank" title="Clique para fazer o download">- MANUAL GESTOR MUNICIPAL</a>';
                        echo '<br><a href="/eja/arquivos/manual_liberar_diretor.pdf" target="_blank" title="Clique para fazer o download">- MANUAL P/ LIBERAR DIRETOR ESCOLAR</a>';
                    ?>
                </td>
            </tr>
        </table>
    </div>

    <br />
    <br />
       
    <div class="page-header">
        <h4 id="forms">Selecione uma Escola abaixo:</h4>
    </div>
    
    <br />
    
    <div class="well">
        <?php
            $cabecalho = array( "Código", "Escola" );
            $sql = "
                SELECT DISTINCT
                    '<a href=\"eja.php?modulo=inicio&acao=C&requisicao=selecionarEscola&entid='|| e.entid ||'\">' || e.entid || '</a>' AS codigo,
                    '<a href=\"eja.php?modulo=inicio&acao=C&requisicao=selecionarEscola&entid='|| e.entid ||'\">' || e.entnome || '</a>' AS nome
                FROM
                    eja.usuarioresponsabilidade ur 
                    INNER JOIN entidade.entidade e ON e.entid = ur.entid
                WHERE
                    ur.usucpf = '".$cpf."'
                    AND ur.pflcod IN( ".join($listaPerfil,',')." )
                    AND ur.rpustatus='A'
            ";

            $listagem = new Simec_Listagem();
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        ?>
        
        <?php if (false === $listagem->render()): ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                Nenhum registro encontrado
            </div>
        <?php endif; ?>
    </div>
</div>