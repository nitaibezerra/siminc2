<?php

// carrega as funções gerais
include_once "config.inc";
include_once "_constantes.php";
include_once '_funcoes.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
// atualiza ação do usuário no sistema
include APPRAIZ . "includes/registraracesso.php";

class Ajax {

    public $db;

    public function __construct($db = false)
    {
        if($db){
            $this->db = new cls_banco();
        }
    }

    public function finalizaFolhaPagamento($post)
    {
        $db = new cls_banco();

        extract($post);
        $muncod = $_SESSION['piso']['muncod'];

        $stWhere = "";
        if($flpanoreferencia){
            $stWhere .= " AND flp.flpanoreferencia = '{$flpanoreferencia}' ";
        }
        if($flpmesreferencia){
            $stWhere .= " AND flp.flpmesreferencia = '{$flpmesreferencia}' ";
        }
        if(!empty($muncod)){
            $stWhereMuncod = " AND pi.muncod = '{$muncod}' ";
        }

        $sql = "SELECT
                    count(flp.flpid)
                FROM pisosalarial.folhapagamento flp
                INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = flp.pmuid
                WHERE (SELECT
                           count(*)
                       FROM pisosalarial.folhapagamentoprofissionais ff
                       WHERE flp.flpid = ff.flpid) > 0
                    {$stWhere}
                    {$stWhereMuncod}";
        $boExiste = $db->pegaUm($sql);

        if($boExiste){
            if($_REQUEST['acao'] == 'abrir'){
                $acao = 'f';
            }else{
                $acao = 't';
            }

            $sql = "SELECT pmuid FROM pisosalarial.pisomunicipio WHERE muncod = '{$muncod}'";
            $pmuid = $db->pegaUm($sql);

            if($pmuid){
                $sql = "UPDATE pisosalarial.folhapagamento flp SET flpfinalizado = '{$acao}' WHERE pmuid = {$pmuid} {$stWhere}";
                $db->executar($sql);
                if($db->commit()){
                    echo 'true';
                }
            }
        }else{
            echo 'sem';
        }


    }

    public function verificaAntesDuplicar()
    {

        $db = new cls_banco();

        $sql = "SELECT count(*) FROM pisosalarial.folhapagamentoprofissionais fp
                INNER JOIN pisosalarial.folhapagamento f ON fp.flpid = f.flpid
                WHERE f.flpanoreferencia = '{$_POST['anodup']}'
                AND f.flpmesreferencia = '{$_POST['mesdup']}'
                AND fp.fppcpf IN (
                    SELECT fpp.fppcpf FROM pisosalarial.folhapagamentoprofissionais fpp
                    WHERE fpp.fppid IN (".implode(', ',$_POST['fppid']).")
                )";



        echo $db->pegaUm($sql);
        exit;
    }

    public function duplicarProfissionaisFolha()
    {
        $db = new cls_banco();

        if(count($_REQUEST['fppid'])){

            $sql = "SELECT
                        f.flpid
                    FROM pisosalarial.folhapagamento f
                    INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = flp.pmuid
                    WHERE f.flpanoreferencia = '{$_REQUEST['anoref']}'
                    AND f.flpmesreferencia = '{$_REQUEST['mesref']}'
                    AND pi.muncod = '{$_SESSION['piso']['muncod']}'";

            $flpid = $db->pegaUm($sql);

            foreach($_REQUEST['fppid'] as $key => $value){

                $sql = "SELECT
                            fppid
                        FROM pisosalarial.folhapagamentoprofissionais p
                        WHERE p.flpid = {$flpid}
                        AND fppcpf = (SELECT fppcpf FROM pisosalarial.folhapagamentoprofissionais p2 WHERE p2.fppid = {$value})";

                $boExiste = $db->pegaUm($sql);

                if(!$boExiste){
                    if(!empty($_REQUEST['fppsalbase'][$key]) && !empty($_REQUEST['fppadigrat'][$key])){

                        $fppsalbase = str_replace(',','.',str_replace('.','',$_REQUEST['fppsalbase'][$key]));
                        $fppadigrat = str_replace(',','.',str_replace('.','',$_REQUEST['fppadigrat'][$key]));

                        $sql = "INSERT INTO pisosalarial.folhapagamentoprofissionais
                                    (carid, forid, vicid, sitid, sebid, lotid, etpid, fppcpf, fppnome, fppcargahoraria, fppzona, fpporgao, fppdesclotacao, entcodent, flpid, fppsalbase, fppadigrat)
                                (SELECT
                                    carid, forid, vicid, sitid, sebid, lotid, etpid, fppcpf, fppnome, fppcargahoraria, fppzona, fpporgao, fppdesclotacao, entcodent, '{$flpid}' as flpid, {$fppsalbase} as fppsalbase, {$fppadigrat} as fppadigrat
                                 FROM pisosalarial.folhapagamentoprofissionais p
                                 INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
                                 WHERE p.fppid = '{$value}')";

                        $db->executar($sql);
                    }
                }
            }
            $db->commit();
        }
//        var_dump($_REQUEST);
//        die('aqui');
    }

    public function deletarProfissionalFolha()
    {
        $db = new cls_banco();

        extract($_POST);

        $sql = "DELETE FROM pisosalarial.folhapagamentoprofissionais WHERE fppid = {$fppid}";

        $db->executar($sql);

        if($db->commit()){ echo 'true'; }else{ echo 'false'; }
    }

    public function recuperarMesesFolhaPagamento()
    {

        $db = new cls_banco();

        $sql = "SELECT DISTINCT
                    m.fmrmescod as codigo,
                    trim(m.fmrmesdsc) as descricao
                FROM pisosalarial.folhamesreferencia m
                INNER JOIN pisosalarial.folhaanoreferencia a ON a.farid = m.farid
                WHERE a.farano = '{$_REQUEST['ano']}'
                AND fmrid NOT IN (SELECT
                                fmrid
                              FROM pisosalarial.folhamesreferencia mm
                              INNER JOIN pisosalarial.folhaanoreferencia aa ON aa.farid = mm.farid
                              WHERE aa.farano = {$_REQUEST['anoref']}
                              AND mm.fmrmescod = {$_REQUEST['mesref']})
                AND m.fmrid NOT IN (SELECT fmrid FROM pisosalarial.pisomunicipio pm
                               INNER JOIN pisosalarial.folhapagamento p ON p.pmuid = pm.pmuid
                               INNER JOIN pisosalarial.folhaanoreferencia aa ON aa.farano = p.flpanoreferencia
                               INNER JOIN pisosalarial.folhamesreferencia mm ON mm.fmrmescod = p.flpmesreferencia AND aa.farid = mm.farid
                               WHERE pm.muncod = '{$_SESSION['piso']['muncod']}'
                               AND p.flpfinalizado = 't')
                ORDER BY m.fmrmescod";

        $rsMeses = $db->carregar($sql);

        foreach($rsMeses as $meses){
            $arMeses[$meses['codigo']] = utf8_encode($meses['descricao']);
        }
        echo simec_json_encode($arMeses);
    }

    public function inserePontuacaoParametro()
    {
        // Objetos
        $db = new cls_banco();

        // Parametros
        $celula       = explode('_', $_POST['celula']);
        $nu_pontuacao = str_replace('P', '', $celula[0]);
        $nu_coluna    = str_replace('C', '', $celula[1]);
        $co_tabela    = $_POST['co_tabela'];

        // Recupera tabela de pontuação
        $sql = "SELECT * FROM pisosalarial.parametrospontuacaotabelas WHERE co_tabela = {$co_tabela}";
        $rsTabela = $db->pegaLinha($sql);

        // Cabeçalho
        echo '<table width="100%" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
                <tr bgcolor="#cccccc">
                   <td align="center" height="30">
                        <b>CADASTRAR PARÂMETRO</b>
                   </td>
                </tr>
              </table>
              <table width="100%" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
                <tr bgcolor="#E9E9E9">
                   <td align="center">

                   </td>
                </tr>
              </table><br/>';

        // Campos
        $arVariaveis = array(array('codigo'     => empty($celula[2]) ? $rsTabela['sg_tabela'] : $celula[2],
                                   'descricao'  => empty($celula[2]) ? $rsTabela['sg_tabela'] : $celula[2]),
                             array('codigo'     => 'numerico',
                                   'descricao'  => 'Campo numérico'));

        $arOperadores = array(array('codigo'=>'>','descricao'=>'Maior que'),
                             array('codigo'=>'>=','descricao'=>'Maior ou igual à'),
                             array('codigo'=>'<','descricao'=>'Menor que'),
                             array('codigo'=>'<=','descricao'=>'Menor ou igual à'),
                             array('codigo'=>'ou','descricao'=>'Ou'));

        // Monta combos
        echo '<table width="100%"><tr><td align="left" valign="top"><div style="width:160px;text-align:left;">Variável</div>';
        $db->monta_combo_multiplo('variavel',$arVariaveis, 'S','','',null,'', 6, 160);
        echo '</td><td align="right" valign="top"><div style="width:160px;text-align:left;">Operador</div>';
        $db->monta_combo_multiplo('operador',$arOperadores,'N','','',null,'', 6, 160);
        echo '</td></tr></table>';

        echo '<div style="height:45px;">
                <div id="insere_valor" style="margin:5px;display:none;">
                    Valor:<br/>
                    <input type="text" name="numerico" value="" class="normal" onkeypress="return somenteNumeros(event);"/>
                    '.(in_array($co_tabela, array(4,5,6)) ? '%' : '').'
                    &nbsp;
                    <a href="javascript:void(0)" class="inserir">Inserir</a>
                </div>
                &nbsp;
              </div>
              <div style="margin:5px;">
                Fórmula
                &nbsp;
                <a href="javascript:void(0)" class="voltar_um">
                    <img border="0" src="../imagens/recuo_e.gif" align="absmiddle" alt="Volta um" title="Voltar um"/></a>
              </div>
              <div style="margin:5px;border: 1px solid #c0c0c0;padding:5px;overflow:auto;" id="formula">
              </div>
              <br/>';

        echo '<input type="hidden" name="co_tabela" value="'.$co_tabela.'" />
              <input type="hidden" name="nu_coluna" value="'.$nu_coluna.'" />
              <input type="hidden" name="nu_pontuacao" value="'.$nu_pontuacao.'" />
              <input type="hidden" name="ds_parametro" value="" />

              <table width="100%" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
                <tr bgcolor="#cccccc">
                   <td align="left">
                     <input type="button" value="Salvar" id="'.$_POST['celula'].'" class="salvaParametro" style="float:left;margin-right:5px;"/>
                     <input type="button" value="Fechar" class="close" style="float:left;margin-right:5px;"/>
                     <input type="button" value="Limpar" class="limpar" style="float:left;margin-right:5px;"/>
                   </td>
                </tr>
              </table>';
    }

    public function gravaParametro()
    {
        $db = new cls_banco();

        $sql = "SELECT
                    ds_parametro
                FROM pisosalarial.parametrospontuacao
                WHERE co_tabela = {$_POST['co_tabela']}
                AND nu_coluna = {$_POST['nu_coluna']}";

        $ds_parametro = $db->pegaUm($sql);

        if(trim($ds_parametro) != trim($_POST['ds_parametro'])){

            // Busca parâmetro
            $sql = "SELECT
                        co_pontuacao
                    FROM pisosalarial.parametrospontuacao
                    WHERE co_tabela = {$_POST['co_tabela']}
                    AND nu_coluna = {$_POST['nu_coluna']}
                    AND nu_pontuacao = {$_POST['nu_pontuacao']}";

            $co_pontuacao = $db->pegaUm($sql);

            // Se existir o parâmetro atualiza se não insere
            if($co_pontuacao){

                $sql = "UPDATE pisosalarial.parametrospontuacao
                            SET ds_parametro = '{$_POST['ds_parametro']}'
                        WHERE co_tabela = {$_POST['co_tabela']}
                        AND nu_coluna = {$_POST['nu_coluna']}
                        AND nu_pontuacao = {$_POST['nu_pontuacao']}";

                $db->executar($sql);

            }else{

                $sql = "INSERT INTO pisosalarial.parametrospontuacao
                            (co_tabela, nu_coluna, nu_pontuacao, ds_parametro)
                        VALUES
                            ({$_POST['co_tabela']}, {$_POST['nu_coluna']}, {$_POST['nu_pontuacao']}, '{$_POST['ds_parametro']}')";

                $db->executar($sql);
            }

            if($db->commit()){
                echo "true";
            }else{
                echo "false";
            }
        }else{
            echo "existe";
        }
    }

    public function verificaMunicipioAnalise()
    {
        $db = new cls_banco();

        $sql = "SELECT
                    d.esdid,
                    e.esddsc,
                    (SELECT count(*) FROM pisosalarial.gestaorecurso g WHERE g.muncod = m.muncod) AS gestaorecursos,
                    (SELECT count(*) FROM pisosalarial.planocarreira p WHERE p.muncod = m.muncod) AS planocarreira
                FROM pisosalarial.pisomunicipio m
                LEFT JOIN workflow.documento d ON d.docid = m.docid
                LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                WHERE m.muncod = '{$_POST['muncod']}'";

        $rsMunicipio = $db->pegaLinha($sql);

        if($rsMunicipio['esdid'] == WF_EM_ANALISE_MEC && $rsMunicipio['gestaorecursos'] > 0 && $rsMunicipio['planocarreira'] > 0){
            echo "true";
        } else{
            echo "false";
        }
    }
}

if(isset($_POST['requisicao'])) {
    $db = ( isset($_POST['db']) && !empty($_POST['db']) ) ? true : false;
    $obAjax = new Ajax($db);
    $obAjax->$_POST['requisicao']($_POST);
}
?>