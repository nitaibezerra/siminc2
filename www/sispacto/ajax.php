<?php

/**
 * Centraliza as requisições ajax do módulo.  
 */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";


// carrega as funções do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

//if($_REQUEST['montar_painel_principal'])
//{
    ?>
    <style>
        .div_conteudo h1{
            color: #15ADFF !important;
        }

        .div_conteudo h1.tituloPainel{
            border-bottom: 1px solid #e5e5e5;
        }
    </style>
    <div class="div_conteudo">
        <div class="head">
            <a href="javascript:void(0);" onclick="javascript:abrirConteudo(this, 'montarPainelWorkFlow');" >
                <h1>
                    <i class="glyphicon glyphicon-plus" ></i>
                    &nbsp; Workflow
                </h1>
            </a>
        </div>
        <div class="body">

        </div>
    </div>
    <div class="div_conteudo">
        <div class="head">
            <a href="javascript:void(0);" onclick="abrirConteudo(this, 'montarPainelFinanceiro');">
                <h1>
                    <i class="glyphicon glyphicon-plus" ></i>
                    &nbsp; Financeiro
                </h1>
            </a>
        </div>
        <div class="body"></div>
    </div>
    <div class="div_conteudo">
        <div class="head">
            <a href="javascript:void(0);" onclick="javascript:abrirConteudo(this, 'montarPainelEstrategico');">
                <h1>
                    <i class="glyphicon glyphicon-plus" ></i>
                    &nbsp; Estatísticas
                </h1>
            </a>
        </div>
        <div class="body"></div>
    </div>
    <script language="javascript">
        function abrirConteudo(elemento , metodo){
            var elemento = $1_11(elemento);
            var icone = elemento.find('i');
            var h1 = elemento.find('h1');
            var corpo = elemento.closest('.head').next('.body');

            h1.addClass('tituloPainel');

            icone.attr('class','glyphicon glyphicon-minus');
            elemento.attr('onclick','javascript:fecharConteudo(this, "' + metodo + '");');
            console.info(window.location.href);

            if($.trim(corpo.html()) == ''){
                $.post(window.location.href, {requisicaoAjaxPainel: metodo} , function(html){
                    corpo.hide().html(html).fadeIn();
                });
                console.info('Carregou ajax.');
            } else {
                corpo.fadeIn();
                console.info('Não carregou ajax de novo.');
            }
        }

        function fecharConteudo(elemento , metodo){
            var elemento = $1_11(elemento);
            var icone = elemento.find('i');
            var h1 = elemento.find('h1');
            var corpo = elemento.closest('.head').next('.body');

            h1.removeClass('tituloPainel');

            icone.attr('class','glyphicon glyphicon-plus');
            elemento.attr('onclick','javascript:abrirConteudo(this, "' + metodo + '");');

            corpo.fadeOut();
        }
    </script>



<?
//    montarPainelWorkFlow();
//    montarPainelFinanceiro();
//    montarPainelEstrategico();

//    die;
//}









