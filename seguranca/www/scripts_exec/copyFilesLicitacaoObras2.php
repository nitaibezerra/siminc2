<?php

if($_REQUEST['gerar']){

    set_time_limit(0);

    define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

    $obras = array();

    $_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
    // $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

    // carrega as funções gerais
    require_once BASE_PATH_SIMEC . "/global/config.inc";
    // require_once "../../global/config.inc";

    require_once APPRAIZ . "includes/classes_simec.inc";
    require_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
    include_once APPRAIZ . "includes/classes/Sms.class.inc";

    //eduardo - envio SMS pendecias de obras - PAR
    //http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
    // CPF do administrador de sistemas
    $_SESSION['usucpforigem'] = '00000000191';
    $_SESSION['usucpf'] = '00000000191';
    $_SESSION['sisid'] = 147;

    $nomeEsquema = 'obras2';
    $db = new cls_banco();

    $sql = "SELECT
                o.obrid,
                o.obrnome,
                a.arqid,
                mu.muncod,
                mu.mundescricao,
                tfl.tfldesc || ' - ' || a.arqnome || '.' || a.arqextensao as nome_arquivo
            FROM obras2.obras o
            JOIN entidade.endereco e ON e.endid = o.endid
            JOIN territorios.municipio mu ON mu.muncod = e.muncod
            JOIN obras2.obralicitacao ol ON ol.obrid = o.obrid AND ol.oblstatus = 'A'
            JOIN obras2.licitacao l ON l.licid = ol.licid AND l.licstatus = 'A'
            LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
            LEFT JOIN obras2.faselicitacao fl ON fl.licid = l.licid AND  fl.flcstatus = 'A'
            JOIN obras2.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid AND tfl.tflstatus = 'A'
            LEFT JOIN obras2.arquivolicitacao al ON al.flcid = fl.flcid AND al.aqlstatus = 'A'
            LEFT JOIN public.arquivo a ON a.arqid = al.arqid
            WHERE
                o.obridpai IS NULL AND
                o.obrstatus = 'A' AND
                a.arqid IS NOT NULL AND
                mu.estuf = 'AM'
          ";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();

    $dadosAgrupados = array();
    foreach($dados as $dado){
        $dadosAgrupados[$dado['mundescricao']]['' .$dado['obrid'] .' - '. $dado['obrnome']][] = $dado;
    }

    $pathRaiz = APPRAIZ."arquivos/".$nomeEsquema.'/copy_licitacoes';
    $origem = APPRAIZ."arquivos/".$nomeEsquema;

    if(!is_dir($pathRaiz)){
        mkdir($pathRaiz, 0777);
    }

    foreach($dadosAgrupados as $municipio => $obras){

        if(!is_dir($pathRaiz . '/' . $municipio)){
            mkdir($pathRaiz . '/' . $municipio, 0777);
        }

        foreach($obras as $obrnome => $obras){

            $obrnome = sanitize(removeAcentos($obrnome));

            if(!is_dir($pathRaiz . '/' . $municipio . '/' . $obrnome)){
                mkdir($pathRaiz . '/' . $municipio . '/' . $obrnome, 0777);
            }

            $pastaFinal = $pathRaiz . '/' . $municipio . '/' . $obrnome;
            foreach($obras as $obra){
                $descricao = removeAcentos($obra['nome_arquivo']);

                $arqid = $obra['arqid'];

                $caminhoOrigem  = $origem . '/' . floor($arqid/1000) . '/' . $arqid;
                $caminhodestino = removeAcentos($pastaFinal . '/' . $obra['nome_arquivo']);

                if(is_file($caminhoOrigem)){
                    $resultado = copy($caminhoOrigem, $caminhodestino);
                }
            }
        }
    }

}


function sanitize($string) {
    $string = preg_replace('/[^a-zA-Z0-9-_\.]/','', $string);
    return $string;
}

echo "FIM";