<?php
/**
 * Sistema de Publicação.
 * werteralmeida
 */
include APPRAIZ . 'includes/workflow.php';
$fm          = new Simec_Helper_FlashMessage('publicacao/principal/solicitacaopublicacao/formSolicitacaopublicacao');
$anexo       = new Publicacao_Model_Anexogeral($_REQUEST['arqid']);
$solicitacao = new Publicacao_Model_Solicitacao($_REQUEST['solid']);
$pathSvnExec = '"C:\\Program Files\\SlikSvn\\bin\\svn.exe"';
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case('downloadAnexo'):
            $resp = $anexo->downloadAnexo($_REQUEST['solicitacao']['solidAnexo']);
            break;
        case 'filtrar':
            $where = " WHERE true ";
            $dados = $_POST['solicitacao'];

            if (isset($dados['usucpf']) && $dados['usucpf'] != '') {
                $where .= " AND usucpf = '{$dados['usucpf']}' ";
            }
            if (isset($dados['soldemanda']) && $dados['soldemanda'] != '') {
                $where .= " AND soldemanda = '{$dados['soldemanda']}' ";
            }
            if (isset($dados['solconteudo']) && $dados['solconteudo'] != '') {
                $where .= " AND solconteudo like '%{$dados['solconteudo']}%' ";
            }
            if (isset($dados['esdid']) && $dados['esdid'] != '') {
                $estados = implode(',', $dados['esdid']);
                $where .= " AND esdid IN ({$estados}) ";
            }

            #ver($dados, $where, d);

            break;
        case 'ver':

            $form = new Simec_View_Form('solicitacao');
            $solicitacao = new Publicacao_Model_Solicitacao($_REQUEST['solid']);
            if (isset($_REQUEST['solid']))
                $dados = $solicitacao->getDados();

            $dados['solconteudo'] = nl2br(str_replace("#", "\\", $dados['solconteudo']));

            $dadosOrdenados = explode("\n", trim($dados['solconteudo']));
            asort($dadosOrdenados);
            $dados['solconteudo'] = implode("\n", $dadosOrdenados);
            $texto = $dados['solconteudo'];
            $linhas = explode("\n", $texto);
            $conta = count($linhas);
            $dados['solconteudo'] = $dados['solconteudo'] . "<br/> <b> Linhas {$conta}</b>";
            $dados['solquery'] = str_replace("\'","'",(str_replace('\"', "'",(nl2br($dados['solquery'])))));
            $dados['soldemanda'] = "<a href=\"http://simec.mec.gov.br/demandas/demandas.php?modulo=principal/observacao&acao=A&dmdid=314196\" target=\"_blank\">{$dados['soldemanda']}</a>";
            $form->addHidden('solid', $dados['solid'])
                    ->addInputTextarea('solconteudo', $dados['solconteudo'], 'solconteudo', 30000, array('somentetexto' => true, 'flabel' => 'Conteúdo'))
                    ->addInputTextarea('soldemanda', $dados['soldemanda'], 'soldemanda', 10, array('somentetexto' => true, 'flabel' => 'Demanda(Simec)'))
                    ->addInputTextarea('solquery', $dados['solquery'], 'solquery', 30000, array('somentetexto' => true, 'flabel' => 'SQL a ser executado'))
                    ->render();
            die(); //'somentetexto'=>true
            break;
        case 'apagar':
            $solicitacao = new Publicacao_Model_Solicitacao($_REQUEST['solid']);
            if ($solicitacao->excluir()) {
                $fm->addMensagem('Solicitação apagada com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            } else {
                $fm->addMensagem('Erro ao apagar', Simec_Helper_FlashMessage::ERRO);
            }

            break;
        case 'vercommit':
            $pedidos = explode(',', $_REQUEST['pedidos']);
            foreach ($pedidos as $pedido) {
                if ($pedido != '') {
                    $solicitacao = new Publicacao_Model_Solicitacao($pedido);
                    $dados = $solicitacao->getDados();
                    $arrArquivos = explode("\n", trim($dados['solconteudo']));
                    foreach ($arrArquivos as $valor) {
                        $endereco = str_replace('#', '\\', $valor);
                        $arquivos .= trim($endereco) . "\n";
                    }
                }
            }
            $arquivos = explode("\n", trim($arquivos));
            asort($arquivos);
            $arquivos = nl2br(implode("\n", $arquivos));
            $texto = $arquivos;
            $linhas = explode("\n", $texto);
            $linhas = count($linhas);
            $arquivos .="<br/> <br/> Arquivos: <b>$linhas</b>";
            $arquivos .= "<input type=\"hidden\" value =\"{$_REQUEST['pedidos']}\" id=\"pedidos-confirmados\"/> ";
            echo $arquivos;
            die();
            break;
        case 'efetuacommit':

            #$urlProd = "D:\\simec\\";
            $urlProd = "D:\\simec-pro\\";

            $pedidos = explode(',', $_REQUEST['pedidos']);
            $mensagem = "Pedido(s) SIMEC: \n";
            foreach ($pedidos as $pedido) {
                if ($pedido != '') {
                    $solicitacao = new Publicacao_Model_Solicitacao($pedido);
                    $dados = $solicitacao->getDados();
                    $mensagem.=$dados['solid'] . ' | ';
                    $arrArquivos = explode("\n", trim($dados['solconteudo']));
                    foreach ($arrArquivos as $valor) {
                        $endereco = str_replace('#', '\\', $valor);
                        $arquivos .= trim($urlProd.$endereco) . "\n";
                    }
                }
            }
            $arquivos = explode("\n", trim($arquivos));
            asort($arquivos);
            $arquivos = nl2br(implode(" ", $arquivos));

            /* Monta a linha do Commit */
            $commitProd = " {$pathSvnExec} commit {$arquivos} -m \"{$mensagem}.\"";
            $svnProdCmt = shell_exec($commitProd);
            #ver($svnProdCmt,$commitProd,d);
            if($svnProdCmt=='')$svnProdCmt="Nenhum arquivo alterado.";
            echo "-------------------------------------------<br/>"
                . "---------- RESPOSTA SVN ---------<br/>"
                . "------------------------------------------<br/>".nl2br($svnProdCmt);
            /* Tramitar para Publicação Solicitada */
            foreach ($pedidos as $pedido) {
                $solicitacao = new Publicacao_Model_Solicitacao($pedido);
                $dados = $solicitacao->getDados();
                $resultado = str_replace("<br />", "\n\r", $svnProdCmt);
                wf_alterarEstado($dados['docid'], 3816, $resultado, array(), array());
                wf_alterarEstado($dados['docid'], 3815, $resultado, array(), array());
            }

            die();
            break;

        case 'executar':
            /* Executar o paranauê */
            $solicitacao = new Publicacao_Model_Solicitacao($_REQUEST['solid']);
            $dados = $solicitacao->getDados();

            $conteudo = $dados['solconteudo'];
            $urlDev = "D:\\simec\\";
            $urlProd = "D:\\simec-pro\\";
            $separador = "\\";

            #ver($urlMaquina,d);
            #$urlMaquina = "d:\\simec\\";

            /* Copiar os arquivos da pasta DEV para a pasta PROD */
            $arrArquivos = explode("\n", trim($conteudo));
            foreach ($arrArquivos as $valor) {
                $endereco = str_replace('#', $separador, $valor);

                $pasta1 = explode($separador, $endereco);
                $pasta2 = array_pop($pasta1);
                $pasta = str_replace($pasta2, '', $endereco);

                $origem = trim($urlDev . $endereco);
                $destino = trim($urlProd . $endereco);

                /* Fazer update da pasta DEV */
                $updateDev = " {$pathSvnExec} update {$urlDev}{$pasta} --force --non-interactive --username werteralmeida --password 55603098Simec";
                $svnDev = shell_exec($updateDev);
                
                /* Criando a pasta em PROD, caso seja nova  */
                if (!is_dir($urlProd . $pasta)) {
//                    ver($urlProd . $pasta, 0777, true);die;
                    mkdir($urlProd . $pasta, 0777, true);
                }

                /* Fazer a cópia do arquivo de DEV > PROD */
                $copiuo = copy("{$origem}", "{$destino}");

                /* Fazer update da pasta PROD */
                $updateProd = " {$pathSvnExec} update {$urlProd}{$pasta} --non-interactive --username werteralmeida --password 55603098Simec";
                $svnProd = shell_exec($updateProd);

                /* Adicionar Arquivo ao SVN PROD */
                $updateProd = " {$pathSvnExec} add $destino --force --parents --username werteralmeida --password 55603098Simec";
                $svnProdAdd = shell_exec($updateProd);
            }

            /* Enviar E-mail p/ o Dedé para ele abrir o Chamado CA */
            #$enviouEmail = enviar_email($remetente, $destinatario, $assunto, $conteudo, $cc, EMAIL_SIMEC_DESENVOLVEDOR, $anexos)

            $solicitacao->solexecutado = 'S';
            if ($copiuo)
                $copiuoRes = " Com sucesso. ";
            $resultado = "

                ------------------------------------------------------------
                -- SVN UPDATE na pasta de Desenvolvimento --
                ------------------------------------------------------------
                {$svnDev}
                ------------------------------------------------------------
                -- Copiar os arquivos da pasta DEV > PROD --
                ------------------------------------------------------------
                {$copiuoRes}
                ----------------------------------------------------
                -- SVN UPDATE na pasta de Produção --
                ----------------------------------------------------
                {$svnProd}
                ----------------------------------------------
                -- SVN ADD na pasta de Produção --
                ----------------------------------------------

                {$svnProdAdd}
";


            $resultado = nl2br($resultado);
            if ($solicitacao->salvar() && $copiuo) {

                $fm->addMensagem("Solicitação EXECUTADA com sucesso {$resultado}", Simec_Helper_FlashMessage::SUCESSO);
                $resultado = str_replace("<br />", "\n\r", $resultado);
                wf_alterarEstado($dados['docid'], 3810, $resultado, array(), array());
            } else {
                $fm->addMensagem("Erro ao EXECUTAR. <br/> <br/> {$resultado}", Simec_Helper_FlashMessage::ERRO);
            }
            break;
        case 'drawWorkflow':
            list(, $docid, $solid) = $_POST['params'];
            wf_desenhaBarraNavegacaoBootstrap($docid, array('docid' => $docid, 'solid' => $solid));
            die();
        case 'historicoBootstrapComentario':
            echo retornaComentarioHistorico($_REQUEST['dados']);
            die;
    }
}


/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$bc = new Simec_View_Breadcrumb('C');
$bc->add('Solicitação', 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A')
        ->add('Listar')
        ->render();
?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>

<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
        $.strPad = function (i, l, s) {
            var o = i.toString();
            if (!s) {
                s = '0';
            }
            while (o.length < l) {
                o = s + o;
            }
            return o;
        };
        
        $('.glyphicon-download-alt').removeClass('btn');
        
        $('.btn-novo').click(function () {
            location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/formSolicitacaopublicacao&acao=A';
        });

        $('.executado').click(function () {
            id = $(this).attr('value');
            bootbox.confirm("Deseja realmente EXECUTAR?", function (result) {
                if (result) {
                    location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=executar&solid=' + id;
                }
            });
        })
        $('.btn-commit').click(function () {
            var conteudo = "Registro não Encontrado";
            pedidos = "";
            $('input[type="checkbox"]:checked').each(function () {
                pedidos += $(this).val() + ", ";
            });
            $.ajax({
                url: "publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=vercommit&pedidos=" + pedidos,
            }).done(function (html) {
                conteudo = html;
                $('.modal-body').html(conteudo);
            });
            $('#modal-ver-commit').modal();
        })

        $('.confirmacommit').click(function () {
            pedidos = $("#pedidos-confirmados").val();
            $.ajax({
                url: "publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=efetuacommit&pedidos=" + pedidos,
            }).done(function (html) {
                conteudo = html;
                bootbox.alert(conteudo,function(){
                    location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A';
                });
            });

        })

        $('.retornarUltimaVersaoSVN').click(function () {
            location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=retornarUltimaVersaoSVN';
        })
            
    });

    function editarPedido(id) {
        location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/formSolicitacaopublicacao&acao=A&solid=' + id;
    }

    function apagaPedido(id) {
        bootbox.confirm("Deseja realmente apagar?", function (result) {
            if (result) {
                location.href = 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=apagar&solid=' + id;
            }
        });
    }
    function verPedido(id) {
        $("#numero-pedido").html($.strPad(id, 5));
        var conteudo = "Registro não Encontrado";
        $.ajax({
            url: "publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A&requisicao=ver&solid=" + id,
        }).done(function (html) {
            conteudo = html;
            $('.modal-body').html(conteudo);
        });
        $('#modal-ver').modal();
    }
    function downloadAnexo( solid ){
            $('#solicitacao').find('#solicitacao_solidAnexo').val(solid);
            $('#solicitacao').find('#action').val('downloadAnexo');
            $('#solicitacao').submit();
    }


</script>
<style>
    .linha_vermelha{
        color:red;
    }
    .linha_amarela{
        color:#f0ad4e;
    }
</style>
<?php
/**
 * Formulário de cadastro e alteração de Solicitação.
 *
 */
$dados = $_REQUEST['solicitacao'];
#ver($_REQUEST);
$dadosSolicitante = "SELECT DISTINCT
    sol.usucpf  AS codigo,
    usu.usunome AS descricao
FROM
    publicacao.solicitacao sol
JOIN
    seguranca.usuario usu
USING
    (usucpf)";
$dadosSituacoes = "
SELECT
    esdid  AS codigo,
    esddsc AS descricao
FROM
    workflow.estadodocumento
WHERE
    tpdid = 239
    ORDER BY 2 ";
$form = new Simec_View_Form('solicitacao',$method = Simec_View_Form::POST, $action = '');
$form->addHidden('solid', $dados['solid'])
       ->addHidden('solidAnexo', '')
       ->addInputCombo('usucpf', $dadosSolicitante, false, 'usucpf', array('flabel' => 'Solicitante'))
       ->addInputTextarea('soldemanda', $dados['soldemanda'], 'soldemanda', 10, array('flabel' => 'Demanda (Simec)', 'rows' => 1, 'cols' => '15'))
       ->addInputTextarea('solconteudo', $dados['solconteudo'], 'solconteudo', 50, array('flabel' => 'Conteúdo da solicitação', 'rows' => 1, 'cols' => '50'))
       ->addInputCombo('esdid', $dadosSituacoes, false, 'esdid', array('flabel' => 'Situação', 'multiple' => true))
       ->addBotoes(array('limpar', 'buscar', 'novo'));
        if(!empty($_REQUEST['solicitacao']['solidAnexo'])){
            $form->setRequisicao('downloadAnexo');
        }else{
            $form->setRequisicao('filtrar');
        }
$form->render();

/* Definir perfis que podem executar */
$podeExecutar = true;
$perfis = pegaPerfilGeral();
if (in_array(1339, $perfis)) {
    $podeExecutar = false;
}

if ($podeExecutar) {

    $botaoCommitProducao = " &nbsp;&nbsp;&nbsp;
    <button class=\"btn btn-danger btn-commit\" type=\"button\">
    <span class=\"glyphicon glyphicon-fire\"></span>
        Commit em Produçao
    </button>";
}

/* Mostrar úlima versão da Produção */
$urlProd = "D:\\simec-pro\\";
$updateProd = " {$pathSvnExec} update {$urlProd}\\teste.php --non-interactive --username werteralmeida --password 55603098Simec";
$svnProd = shell_exec($updateProd);
$saida = explode(' ', $svnProd);
echo "{$botaoCommitProducao}<div style=\"text-align:right; padding-bottom:3px\">Versão atual do SVN de Produção: <b>{$saida[3]}</b></div>";
echo $fm->getMensagens();



$listagem = new Simec_Listagem();
$arrColunas = array('Solicitação', 'Data', 'Solicitante', 'SQL', 'Situação');
if ($podeExecutar) {
    $arrColunas = array('Produção', 'Executar', 'SQL', 'Solicitação', 'Data', 'Solicitante', 'Situação');
    $executar = "CASE WHEN solconteudo <> '' THEN solid||'_e' ELSE '-' END as executado, ";
    $listagem->addCallbackDeCampo('commit', 'checkboxEnviar');
} else {
    $listagem->esconderColuna('commit');
}
$listagem->addAcao('workflow', array( 
                                        'func' => 'drawWorkflow',       
                                        'extra-params' => array('docid', 'solid')
                                    )
                  );
$listagem->addAcao('edit', 'editarPedido');
$listagem->addAcao('delete', 'apagaPedido');
$listagem->addAcao('view', 'verPedido');
$listagem->addAcao('download', 'downloadAnexo');
$listagem->setAcaoComoCondicional( 'download',array(array('campo' => 'arqid', 'valor' => 0, 'op' => 'diferente') ) );

$sql = "
    SELECT
    solid,
    solid as commit,
    sol.docid,
    {$executar} 
    CASE WHEN arqid IS NULL THEN 0 ELSE arqid END as arqid,
    CASE WHEN sol.solquery <> '' THEN 'S' ELSE 'N' END as temsql,
    lpad(solid::text,5,'0') as codigo,
    TO_CHAR(soldata,'dd/mm/yyyy HH24:mi') AS datasol,
    usu.usunome AS solicitante,
    sol.solurgente,
    esd.esddsc
FROM
    publicacao.solicitacao sol
LEFT JOIN 
    publicacao.anexogeral anex 
USING
    (solid)
JOIN
    seguranca.usuario usu
USING
    (usucpf)
left join
    workflow.documento doc
USING(docid)
LEFT JOIN workflow.estadodocumento esd USING(esdid)
$where
ORDER BY
    soldata DESC,
    usu.usunome ASC  ";//ver($sql);

$listagem->setCabecalho($arrColunas)->setQuery($sql)
    ->addCallbackDeCampo('executado', 'tratarBotoesAcoes')
    ->addCallbackDeCampo('temsql', 'tratarSimNao')
    ->esconderColunas('solurgente', 'docid','arqid')
    ->addRegraDeLinha(
            array('campo' => 'solurgente', 'op' => 'igual', 'valor' => 'S', 'classe' => 'linha_vermelha')
    )
    ->addRegraDeLinha(
            array('campo' => 'esddsc', 'op' => 'igual', 'valor' => 'Aguardando publicação', 'classe' => 'linha_amarela')
    )
;
$listagem->turnOnPesquisator()
        ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
?>

<div class="modal fade" id="modal-ver">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Pedido nº <spam id="numero-pedido"></spam></h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
<div class="modal fade" id="modal-ver-commit">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Arquivos a serem enviados para Produção</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-danger confirmacommit" data-dismiss="modal">ENVIAR</button>
            </div>
        </div>
    </div>
</div>