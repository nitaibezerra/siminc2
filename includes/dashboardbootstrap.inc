<?php

/*
 * Funções para a montagem da tela principal 
 * Usando o novo padrão Bootstrap
 * Werter Almeida werteralmeida@mec.gov.br  
 * 
 */

/**
 * Função para montar o botão da tela inicio.
 * 
 * 
 * @param $params['tipo']-> processo, listar, cadastrar, pdf, relatorio, tabelaapoio, baixarws, processar, log, calendario, snapshot, acompanhamento, questionario,
 * central, espelho, user, upload 
 */
function montaBotaoInicio($params) {
    $retorno = "";

    /* Trocando o Ícone */
    switch ($params['tipo']) {
        case 'processo':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-random\" style=\"color: royalblue; font-size: 16px; margin-right: 5px;\" ></span> ";
            break;
        case 'listar':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-list-alt\" style=\"color: #31B404; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'cadastrar':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-file\" style=\"color: #31B404; font-size: 16px; \" ></span> ";
            break;
        case 'pdf':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-book\" style=\"color: red; font-size: 16px; \" ></span> ";
            break;
        case 'relatorio':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-print\" style=\"color: #DAA520; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'tabelaapoio':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-cog\" style=\"color: #8B658B; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'baixarws':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-cloud-download\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'processar':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-retweet\" style=\"color: #02CCD9; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'log':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-list\" style=\"color: #efb73e; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'calendario':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-calendar\" style=\"color: #FF0000; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'snapshot':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-camera\" style=\"color: #000000; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'acompanhamento':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-signal\" style=\"color: #002A6C; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'enviar':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-send\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'questionario':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-question-sign\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'central':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-th\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'user':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-user\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'espelho':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-th-large\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        case 'upload':
            $icone = "<span class=\"iconeBotao glyphicon glyphicon-cloud-upload\" style=\"color: royalblue; font-size: 16px;margin-right:2px;\" ></span> ";
            break;
        default:
            break;
    }

    if (((isset($params['url']) && empty($params['url'])) || (!isset($params['url']) && empty($params['url']))) && ((!isset($params['toggle']) && empty($params['toggle'])) || (isset($params['toggle']) && empty($params['toggle']))) && ((!isset($params['target']) && empty($params['target'])) || (isset($params['target']) && empty($params['target'])))
    )
        $params['class'] = $params['class'] . ' inativo';

    $retorno .= "<div class=\"btnNormal btnOn {$params['class']}\" id=\"{$id}\"";

    if (isset($params['target']))
        $retorno .= "data-target=\"{$params['target']}\"";


    if (isset($params['toggle']))
        $retorno .= "data-toggle=\"{$params['toggle']}\"";

    if (isset($params['url']) && $params['target'] != '_blank')
        $retorno .= "data-request=\"{$params['url']}\"";

    if (isset($params['url']) && $params['target'] == '_blank')
        $retorno .= "data-request=\"#\"";

    if (isset($params['target']) && $params['target'] != '_blank')
        $retorno .= "data-target=\"{$params['target']}\"";

    $retorno .= ">";

    $retorno .= "{$icone}";
    $retorno .= "{$params['texto']}";
    $retorno .= "</div>";
    if (isset($params['target']) && $params['target'] == '_blank') {
        $retorno = "<a href='{$params['url']}' target='_blank' >{$retorno}</a>";
    }
    echo $retorno;
}

function montaComunicados() {
    include_once APPRAIZ . 'includes/SpoComunicados/classes/Comunicado.php';
    $modulo = MODULO;
    $listaPerfil = pegaperfilgeral();
    
    if(
        in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $listaPerfil)
        || in_array(PFL_ADMINISTRADOR, $listaPerfil)
    ){
        $params = array();
        $params['texto'] = 'Editar Comunicados';
        $params['tipo'] = 'listar';
        $params['url'] = "{$modulo}.php?modulo=principal/comunicado/listar&acao=A";
        montaBotaoInicio($params);

        $params = array();
        $params['texto'] = 'Cadastrar Comunicado';
        $params['tipo'] = 'cadastrar';
        $params['url'] = "{$modulo}.php?modulo=principal/comunicado/cadastrar&acao=A";
        montaBotaoInicio($params);
        
    echo <<<HTML
    <span class="subTituloCaixa">&nbsp;</span>
HTML;
    }
    
    $comunicado = new Comunicado($modulo);
    $listaComunicado = $comunicado->listaInicial();
    if ($listaComunicado !== FALSE) {
        $listaComunicado = is_array($listaComunicado) ? $listaComunicado : Array();

        foreach ($listaComunicado as $data):

            $params = array();
            $params['texto'] = $data['angdsc'];
            $params['tipo'] = 'pdf';
            $params['url'] = "javascript: onclick = abrirArquivo('{$data['arqid']}'); ";
            montaBotaoInicio($params);

        endforeach;
    }else {
        echo "<p style=\"clear:both;color:#FFF\"><br/> Não existem comunicados cadastrados.</p>";
    }
}
