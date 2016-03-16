<?
pegaSessoes();

if ($_REQUEST["prtid"]) {
    include_once APPRAIZ . "www/academico/_permissoes.php";
}
$orgid = $_SESSION['academico']['orgid'];
$ano = $_SESSION['academico']['ano'];
$arqid = $_REQUEST['arqid'];
$anxid = $_REQUEST['anxid'];
$unidade = $_REQUEST['unidade'];
$prgid = $_REQUEST['prgid'];
$orgdesc = ($orgid == 1) ? "Educação Superior" : "Educação Profissional";

unset($_SESSION["academico"]["prtidautprov"]);

// Recebe o ID do registro para editar
if ($_REQUEST["prtid"]) {
    $_SESSION["academico"]["prtid"] = (integer) $_REQUEST["prtid"];
}

# Limpa a sessão para inserir um novo registro
if ($_REQUEST[evento] == 'N')
    unset($_SESSION["academico"]["prtid"]);

// Pega o (pai) e Tipo
if (isset($_SESSION["academico"]["prtid"])) {

    $sql = "SELECT tprid, prtidautprov
			FROM academico.portarias AS prt
			WHERE prtid =" . $_SESSION["academico"]["prtid"];
    $dados = $db->pegaLinha($sql);

    if (isset($_REQUEST["prtidautprov"]) and $_REQUEST["prtidautprov"] != '')
        $_SESSION["academico"]["prtidautprov"] = $_REQUEST["prtidautprov"];
    else
        $_SESSION["academico"]["prtidautprov"] = $dados["prtidautprov"];

    if (isset($_REQUEST["tprnivel"]) and $_REQUEST["tprnivel"] != '')
        $_SESSION["academico"]["tprnivel"] = $_REQUEST["tprnivel"];
    else
        $_SESSION["academico"]["tprnivel"] = $dados["tprid"];
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if ($_SESSION["academico"]["prtid"]) {
    echo montarAbasArray(carregardadosmenuacademico('dadosPortaria'), '/academico/academico.php?modulo=principal/cadportaria&acao=' . $_REQUEST['acao']);
//	$db->cria_aba($abacod_tela,$url,'');
}

monta_titulo("Dados da Portaria", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
if ($_SESSION["academico"]["prtid"] and $_SESSION["academico"]["prtid"] != '' and $_REQUEST[evento] != 'N')
    echo $autoriazacaoconcursos->cabecalho_ini($_SESSION["academico"]["prtid"]);


if (isset($_SESSION["academico"]["prtid"]) and $_SESSION["academico"]["prtid"] != '') {
    if (isset($_REQUEST[evento_arquivo]) && ($_REQUEST[evento_arquivo] != '')) {
        switch ($_REQUEST[evento_arquivo]) {
            case 'EA':
                $dados_arq = array('arqid' => $arqid, 'anxid' => $anxid);
                DeletarDocumento($dados_arq);
                echo "<script>
                            alert('Dados salvos com sucesso.');
                            window.location = '?modulo=principal/cadportaria&acao=C&evento=A&prtid=" . $_SESSION["academico"]["prtid"] . "&orgid=" . $orgid . "';
                      </script>";
                exit;
                break;
            case 'DA':
                $dados_arq['arqid'] = $_REQUEST['arqid'];
                DownloadArquivo($dados_arq);
                exit;
                break;

            case 'SA':
                $arquivo = $_FILES['arquivo'];
                $arqdescricao = $_REQUEST['arqdescricao'];
                $tpaid = $_REQUEST['tpaid'];
                $dados_arq = array('arqdescricao' => $arqdescricao, 'tpaid' => $tpaid, 'prtid' => $_SESSION["academico"]["prtid"]);
                //dbg($dados_arq,1);
                if (isset($arquivo['name']) && $arquivo['name'] != '') {
                    EnviarArquivo($arquivo, $dados_arq);
                }
                echo "<script>
                            alert('Dados salvos com sucesso.');
                            window.location = '?modulo=principal/documentos&acao=C&evento=A&prtid=" . $_SESSION["academico"]["prtid"] . "&orgid=" . $orgid . "';
                      </script>";
                exit;
                break;
        }
    }

    // Verifica se ainda pode ser alterado o registro
    # Registro que ainda não possui arquivo gerado e nem número de portaria.
    $sql = "SELECT
                    pta.prtid
            FROM
                    public.arquivo arq
                    INNER JOIN academico.anexos aqb ON aqb.arqid = arq.arqid
                    INNER JOIN academico.portarias pta ON pta.prtid = aqb.prtid
            WHERE
                    pta.prtid = " . $_SESSION["academico"]["prtid"] . " AND
                    aqb.anxstatus = 'A' AND arqdescricao LIKE 'Portaria'";
    $arquivos_gerados = $db->pegaUm($sql);
}

if (!$_REQUEST[evento])
    $_REQUEST[evento] = 'A';

if ($_REQUEST["unidade"])
    $_SESSION["academico"]["unidade"] = (integer) $unidade;

if ($_REQUEST["orgid"])
    $_SESSION["academico"]["orgid"] = (integer) $orgid;

if ($_REQUEST['download'] == 'S') {
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];

    DownloadArquivo($param);
    exit;
}

switch ($tipo) {
    case 'P':
        $texto = "da Portaria";
        $texto2 = "de Portarias";
        break;
    case 'E':
        $texto = "do Edital";
        $texto2 = "de Editais";
        break;
}

if (isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')) {
    switch ($_REQUEST[evento]) {
        case 'N':

            unset($_SESSION['academico']['prtid']);
            break;
        case 'I':

            if (isset($_REQUEST['nasid']) && ($_REQUEST['nasid'] != '')) {
                $nasid = $_REQUEST['nasid'];
            } else {
                $nasid = 'null';
            }

            if (isset($_REQUEST['prtdtpublicdou']) && ($_REQUEST['prtdtpublicdou'] != '')) {
                $prtdtpublicdou = "'" . formata_data_sql($_REQUEST['prtdtpublicdou']) . "'";
            } else {
                $prtdtpublicdou = 'null';
            }

            if (isset($_REQUEST['prtdtportaria']) && ($_REQUEST['prtdtportaria'] != '')) {
                $prtdtportaria = "'" . formata_data_sql($_REQUEST['prtdtportaria']) . "'";
            } else {
                $prtdtportaria = 'null';
            }
            /* if(isset($_REQUEST['prtidautprov']) && ($_REQUEST['prtidautprov'] != '')){
              $prtidautprov = $_REQUEST['prtidautprov'];
              }else{
              $prtidautprov = 'null';
              } */

            $prtid_i = $_SESSION['academico']['prtid'];
            $usucpf = $_SESSION[usucpf];
            $prtnumero = $_REQUEST['prtnumero'];
            $prtano = $ano;
            $prttipoportaria = $_REQUEST['tprid'];
            $prtsecaodiario = isset($_REQUEST['prtsecaodiario']) ? $_REQUEST['prtsecaodiario'] : 'null';
            $prtpaginadiario = isset($_REQUEST['prtpaginadiario']) ? $_REQUEST['prtpaginadiario'] : 'null';
            $prtnumdiario = $_REQUEST['prtnumdiario'];
            $prtnumprovimentos = (isset($_REQUEST['prtnumprovimentos']) && ($_REQUEST['prtnumprovimentos'] != '')) ? $_REQUEST['prtnumprovimentos'] : 'null';
            $prgid = (isset($_REQUEST['prgid']) && ($_REQUEST['prgid'] != '')) ? $_REQUEST['prgid'] : 'null';
            $arquivo = $_FILES['arquivo'];
            $tpaid = $_REQUEST['tpaid'];
            $arqdescricao = $_REQUEST['arqdescricao'];
            $prtidautprov = $_REQUEST['prtidautprov'] ? $_REQUEST['prtidautprov'] : 'null';

            $dados_arq = array('arqdescricao' => $arqdescricao, 'tpaid' => $tpaid, 'prtid' => $_SESSION["academico"]["prtid"]);

            if ($_SESSION["academico"]["prtid"] and $_SESSION["academico"]["prtid"] != '') {
                $dados_arq[prtid] = $_SESSION["academico"]["prtid"];
                $sql = "
                    SELECT
                            p.prtid
                    FROM  academico.portarias p
                    WHERE p.prtid = " . $_SESSION["academico"]["prtid"];
                $dados = $db->pegaUm($sql);
            }
            // caso não exista o edital/portaria insere um novo
            if (!$dados) {

                $sql_I = "INSERT
                            INTO academico.portarias
                                    (usucpf, orgid, prgid, prtnumero,prtano,prtdtportaria,
                                     nasid,tprid,prtsecaodiario,prtpaginadiario,
                                    prtdtpublicdou, prtnumdiario, prtdtinclusao, prtidautprov)
                            VALUES
                                     ('" . $usucpf . "', '" . $orgid . "', " . $prgid . ", '" . $prtnumero . "','" . $prtano . "', " . $prtdtportaria . ",
                                           " . $nasid . ", " . $prttipoportaria . ", '" . $prtsecaodiario . "', '" . $prtpaginadiario . "',
                                          " . $prtdtpublicdou . ", '" . $prtnumdiario . "', now(), " . $prtidautprov . ")
                            RETURNING	prtid
                ";

                $dados_arq[prtid] = $db->pegaUm($sql_I);

                $_SESSION["academico"]["prtid"] = $dados_arq[prtid];

                if (isset($arquivo['name']) && $arquivo['name'] != '') {
                    EnviarArquivo($arquivo, $dados_arq);
                }
                //caso exista o indicador altera os dados do mesmo
            } else {
                $prgid = isset($_REQUEST['prgid']) ? $_REQUEST['prgid'] : 'null';
                $sql_U = "
                    UPDATE academico.portarias
                    SET
                     prtnumero	 			= '" . $prtnumero . "',
                     prtano		 			= '" . $ano . "',
                     prgid					= " . $prgid . ",
                     prtdtportaria	 		= " . $prtdtportaria . ",
                     nasid			  		= " . $nasid . ",
                     tprid			  		= " . $prttipoportaria . ",
                     prtsecaodiario			= '" . $prtsecaodiario . "',
                     prtpaginadiario		= '" . $prtpaginadiario . "',
                     prtnumdiario 			= '" . $prtnumdiario . "',
                     prtdtpublicdou			= " . $prtdtpublicdou . ",
                     prtidautprov			= " . $prtidautprov . "
                     WHERE
                     prtid 					= " . $_SESSION["academico"]["prtid"] . "
                ";

                $db->executar($sql_U);
                if (isset($arquivo['name']) && $arquivo['name'] != '') {
                    EnviarArquivo($arquivo, $dados_arq);
                }
                // Salva as novas posições na ordem correta
                $atualiza_ordem = $_REQUEST["atualiza_ordem"];
                $atualiza_ordem = explode(",", $atualiza_ordem);
                $qRegs = count($atualiza_ordem);
                
                for ($i = 1; $i < $qRegs; $i++) {
                    $atualiza_ordem_un = explode("_", $atualiza_ordem[$i]);
                    $id_alterar = $atualiza_ordem_un[0];
                    $ordem_alterar = $atualiza_ordem_un[1];
                    $sql_U = "
                        UPDATE academico.composicaoportaria
                        SET copordem = " . $ordem_alterar . "
                        WHERE copid = " . $id_alterar;
                    $db->executar($sql_U);
                }
            }

            $db->commit();
            if (!$_REQUEST["msg_nao_mostrar"]) {
                echo "<script>
                            alert('Dados salvos com sucesso.');
                            window.location = '?modulo=principal/cadportaria&acao=C&evento=A&prtidautprov=" . $prtidautprov . "&prtid=" . $_SESSION["academico"]["prtid"] . "&orgid=" . $orgid . "';
                      </script>";
            } else {
                echo "<script>
                            window.location = '?modulo=principal/cadportaria&acao=C&evento=A&prtidautprov=" . $prtidautprov . "&prtid=" . $_SESSION["academico"]["prtid"] . "&orgid=" . $orgid . "';
                      </script>";
            }

            exit;
            break;

        case 'E':
            $prtid = $_REQUEST['prtid'];

            $sql = "SELECT
                            prtidautprov
                    FROM
                            academico.portarias
                    WHERE
                            prtid = {$prtid}";
            $prtidautprov = $db->pegaUm($sql);

            $sql_D = " UPDATE academico.portarias SET prtstatus = 'I' where prtid = " . $prtid . "";
            $db->executar($sql_D);
            $db->commit();

            echo "<script>
                        alert('Dados excluidos com sucesso.');
                        window.location = '?modulo=principal/cadportaria&acao=C&evento=A&prtid=" . $prtidautprov . "';
                  </script>";

            exit;
            break;

        case 'A':

            if ($_SESSION["academico"]["prtid"]) {
                $sql = "
                    SELECT
                            p.usucpf,
                            p.prtnumero,
                            p.prgid,
                            prtdtportaria,
                            nasid,
                            tprid,
                            prtsecaodiario,
                            prtpaginadiario,
                            p.prtdtpublicdou,
                            p.prtano,
                            p.prtnumdiario,
                            p.prtdtinclusao,
                            prtidautprov
                    FROM  academico.portarias p
                    WHERE p.prtid = " . $_SESSION["academico"]["prtid"];
                $dados = $db->pegaLinha($sql);

                $usucpf = $dados['usucpf'];
                $prtnumero = $dados['prtnumero'];
                $prtano = $dados['prtano'];
                $prgid = $dados['prgid'];
                $prtdtportaria = formata_data($dados['prtdtportaria']);
                $prtnumprovimentos = $dados['prtnumprovimentos'];
                $nasid = $dados['nasid'];
                $tprid = $dados['tprid'];
                $prtsecaodiario = $dados['prtsecaodiario'];
                $prtpaginadiario = $dados['prtpaginadiario'];
                $prtnumdiario = $dados['prtnumdiario'];
                $prtdtpublicdou = formata_data($dados['prtdtpublicdou']);
                $prtidautprov = $dados['prtidautprov'];

                if (isset($prtidautprov) and $prtidautprov != null)
                    $_REQUEST["prtidautprov"] = $prtidautprov;
            }
            break;
    }
}

$portaria = "portaria_" . $prtnumero . "_" . $ano;

if (isset($_SESSION["academico"]["prtid"]) and $_SESSION["academico"]["prtid"] != "") {
    $sql = "
        SELECT
                DISTINCT(cp.copid) as id_reg
        FROM academico.composicaoportaria cp
        INNER JOIN academico.tipocomposicao tp ON cp.tpcid = tp.tpcid
        INNER JOIN academico.lancamentosportaria lp ON lp.prtid = cp.prtid
        WHERE cp.prtid=" . $_SESSION["academico"]["prtid"] . " AND tp.tpcid in (1,4)
        GROUP BY cp.copid";
    $gerar_portaria = $db->pegaUm($sql);
}

function montaTabela($dis) {
    global $habilitado;
    global $db;

    if( isset($_SESSION["academico"]["prtid"]) && $_SESSION["academico"]["prtid"] != "" ){

        $query = "
            SELECT  cp.copid as id_reg,
                    cp.prtid as id,
                    cp.copordem as ordem,
                    tp.tpcid as idtipo,
                    tp.tpcdesc as tipo,
                    cp.copdesc as conteudo
            FROM academico.composicaoportaria cp

            INNER JOIN  academico.tipocomposicao tp ON cp.tpcid = tp.tpcid
            WHERE prtid = {$_SESSION["academico"]["prtid"]} AND tp.tpcid in (1,4,5)

            ORDER BY cp.copordem
        ";
        $sql = $db->executar($query);
        
        $count = 1;
        $controleLinha = 1;
        $cont_art = 1;
        $atualiza_quant_artigos = 0;

        while (($dados = pg_fetch_array($sql)) != false) {
            $id_registro = $dados['id_reg'];
            $ordem = $dados['ordem'];
            $tipo = $dados['tipo'];
            $conteudo = $dados['conteudo'];
            $copid = $dados['id_reg'];
            $cor = "#f4f4f4";
            $nome = $count;
            $idtipo = $dados['idtipo'];
            $count++;

            if($count % 2) {
                $cor = "#e0e0e0";
            }

            if($itcdescservico != ''){
                $title = "onmouseover=\"return escape('$itcdescservico');\"";
            }else{
                $title = "";
            }

            $botaoExcluir = "<span><img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";

            if($dis == ""){
                if ($dados_e["num"] == 0) {
                    $botaoExcluir = " 
                        <span onclick='excluiItem(this.parentNode.parentNode.rowIndex,$copid);'>
                            <img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'>
                        </span>
                    ";
                    $botaoEditar = "
                        <span onclick='editarPartePrortaria($id_registro," . $_SESSION["academico"]["prtid"] . ",0);'>
                            <img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Editar'>
                        </span>
                    ";
                } else {
                    //$botaoExcluir = "<span onclick='alert(\"Existe supervisão cadastrada para esta etapa.\");'><img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
                }
            }

            if (!$habilitado) {
                $detalhesSetaCima = "<span><a ><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
                $detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
            } else {
                if ($controleLinha == 1) {
                    $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
                    if (pg_num_rows($sql) == 1) {
                        $detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
                    } else {
                        $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
                    }
                } elseif (pg_num_rows($sql) == $controleLinha) {
                    $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
                    $detalhesSetaBaixo = "<span><a onclick=\"\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
                } else {
                    $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
                    $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
                }
            }
            echo "<tr id=\"$nome\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">";

            if ($arquivos_gerados != null) {
                $detalhesSetaCima = "";
                $detalhesSetaBaixo = "";
                $botaoExcluir = "";
                $botaoEditar = "";
            }

            echo "
                <td align=\"center\">
                    $detalhesSetaCima
                    $detalhesSetaBaixo
		</td>
		<td align=\"center\">
                    $botaoExcluir  $botaoEditar
		</td>
            ";
            echo"<td $title align=center>";

            if ($idtipo == 1) {
                echo "$tipo $cont_art °";
                $cont_art++;
            } else
                echo "$tipo";
            echo "
                    <input type=hidden id=ordem_$nome name=ordem_linha[] value='$id_registro'>
                </td>
                <td $title>
                    <div style='max-height: 40px; overflow-y: scroll;'>
                        $conteudo
                    </div>
            ";

            if ($idtipo == 1) {
                echo "<center style='background: gray;'><font color=white>Incisos e Parágrafos</font></center>";

                $query2 = "
                    SELECT  cp.copid as id_reg,
                            cp.prtid as id,
                            cp.copordem as ordem,
                            tp.tpcid as idtipo,
                            tp.tpcdesc as tipo,
                            cp.copdesc as conteudo
                    FROM academico.composicaoportaria cp 
                    
                    INNER JOIN  academico.tipocomposicao tp ON cp.tpcid = tp.tpcid
                    
                    WHERE cp.copidinciso = {$id_registro}
                        
                    ORDER BY cp.copordem
                ";
                $sql2 = $db->executar($query2);
                
                echo "
                    <table cellSpacing=0 cellPadding=1 width=100% id=tabela_PartePrortarias_$atualiza_quant_artigos name=tabela_PartePrortarias_$atualiza_quant_artigos>
                        <tr bgcolor=c0c0c0>
                            <td width=10> <b>Ordem</b></td><td width=10> <b>Ações </b></td><td align=center> <b>Tipo </b></td><td> <b>Conteúdo </b></td>
			</tr>
                ";

                # Tabela de Parágrafos #
                $cont_parag = 1;
                $cont_inci = 1;
                $count2 = 0;
                $controleLinha2 = 1;
                while (($dados2 = pg_fetch_array($sql2)) != false) {
                    $id_registro2 = $dados2['id_reg'];
                    $tipo2 = $dados2['tipo'];
                    $idtipo2 = $dados2['idtipo'];
                    $conteudo2 = $dados2['conteudo'];

                    if ($habilitado) {
                        $botaoExcluir = "<span onclick='excluiItem_inciso($id_registro2);'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
                    } else {
                        $botaoExcluir = "<span onclick='excluiItem_inciso($id_registro2);'><img src=\"/imagens/excluir_01.gif \" border=0 ;\" title=Excluir></span>";
                    }
                    $botaoEditar = "<span onclick='editarPartePrortaria($id_registro2," . $_SESSION["academico"]["prtid"] . "," . $id_registro . ");'><img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Editar'></span>";

                    $count2++;
                    $cor2 = "#E0E0E0";
                    if ($count2 % 2) {
                        $cor2 = "#F4F4F4";
                    }
                    if (!$habilitado) {
                        $detalhesSetaCima = "<span><a ><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
                        $detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
                    } else {
                        if ($controleLinha2 == 1) {
                            $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias_$atualiza_quant_artigos','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
                            if (pg_num_rows($sql2) == 1) {
                                $detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
                            } else {
                                $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias_$atualiza_quant_artigos','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
                            }
                        } elseif (pg_num_rows($sql2) == $controleLinha2) {
                            $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias_$atualiza_quant_artigos','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
                            $detalhesSetaBaixo = "<span><a onclick=\"\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
                        } else {
                            $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias_$atualiza_quant_artigos','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
                            $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_PartePrortarias_$atualiza_quant_artigos','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
                        }
                    }

                    echo "
                        <tr id=\"$count2\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor2';\" >
                            <td>$detalhesSetaCima $detalhesSetaBaixo</td>
                            <td>$botaoExcluir $botaoEditar</td>
                    ";

                    if ($idtipo2 == 3) {
                        echo "<td width=10 align=center>$tipo2</td><td>§ $cont_parag °";
                        $cont_parag++;
                    } else {
                        if ($idtipo2 == 2) {
                            echo "<td width=10 align=center>$tipo2</td><td>" . decimal2romano($cont_inci) . " ";
                            $cont_inci++;
                        } else {
                            echo "<td width=10 align=center>$tipo2</td><td> ";
                        }
                    }

                    echo "
                            <div style='max-height: 40px; overflow-y: scroll;'>$conteudo2</div></td>
                                <td style='display: none;'>$id_registro2</td>
                        </tr>
                    ";
                    $controleLinha2++;
                }

                $atualiza_quant_artigos+=1;
                echo "</table>";

                if($habilitado){
                    echo "<br><a style='color: blue;' href=# onclick='javascript: inserir_inciso($id_registro)'><img src='/imagens/gif_inclui.gif' style='cursor:pointer;' border=0 title=Inserir> Inserir</a>";
                }
            }

            echo "
                    </td> <td style='display: none;'>$id_registro</td>
                </tr>
            ";
            $controleLinha++;
        }
    }
    echo "<input type=hidden id=atualiza_quant_artigos name=_quant_artigos value=$atualiza_quant_artigos>";
}
?>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
    </head>
    <body >

        <form method="POST"  name="formulario" id="formulario">
            <input type="hidden" name="evento" id="evento" value="I">
            <input type="hidden" name="evento_arquivo" id="evento_arquivo" value=""/>
            <input type="hidden" name="tipo" id="tipo" value="<?= $tipo ?>">
            <input type="hidden" name="arqid" id="arqid" value="<?= $arqid ?>">
            <input type="hidden" name="anxid" id="anxid" value="<?= $anxid ?>">
            <input type="hidden" name="msg_nao_mostrar" id="msg_nao_mostrar">
            <input type="hidden" name="prtid" id="prtid" value="<?= $_SESSION["academico"]["prtid"]; ?>">
            <input type="hidden" name="prtidautprov" id="prtidautprov" value="<?= $_REQUEST["prtidautprov"] ? $_REQUEST["prtidautprov"] : $_SESSION["academico"]["prtidautprov"]; ?>">

            <iframe src="" name="cnt" style="display: none"></iframe>
            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td align='right' width="30%" class="SubTituloDireita">Tipo de portaria:</td>
                    <td>
                        <?PHP
                            if ($_SESSION["academico"]["tprnivel"] == ACA_TPORTARIA_CONCURSO){
                                echo "<b>Autorização de Concurso </b><input type=hidden name=tprid id=tprid value=1>";
                            }else{
                                echo "<b>Autorização de Provimentos </b> <input type=hidden name=tprid id=tprid value=2>";
                            }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td align='right' width="30%" class="SubTituloDireita">Número de Controle:</td>
                    <td>
                        <?php
                            echo $_SESSION["academico"]["prtid"];
                        ?>
                    </td>
                </tr>
                <tr>
                    <td align='right' width="30%" class="SubTituloDireita">Programa:</td>
                    <td>
                        <?php
                            if ($_SESSION["academico"]["tprnivel"] == 1) {
                                $sql = "
                                    SELECT  prgid as codigo,
                                            prgdsc as descricao

                                    FROM academico.programa
                                    ORDER BY prgdsc
                                ";
                                $db->monta_combo("prgid", $sql, $habil, "Selecione...", '', '', '', '', 'S', 'prgid');
                            }else{
                                if (isset($prgid) and $prgid <> '') {
                                    $programa = $db->pegaUm("SELECT prgdsc FROM academico.programa pg WHERE prgid=" . $prgid);
                                    echo "<b>$programa</b> <input type=hidden name=prgid id=tprid value=$prgid>";
                                }
                            }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td align='right' width="30%" class="SubTituloDireita">Número <?= $texto ?>:</td>
                    <td><?= campo_texto('prtnumero', 'N', $habil, '', 15, 10, '', ''); ?></td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Data da Portaria:</td>
                    <td><?= campo_data('prtdtportaria', 'N', $habil, '', '', '', ''); ?></td>
                </tr>
                <tr>
                    <td align='right' width="30%" class="SubTituloDireita">Responsável pela assinatura:</td>
                    <td>
                        <?PHP
                            $sql = "
                                SELECT  nasid as codigo,
                                        nasdsc as descricao

                                FROM academico.assinatura
                                ORDER BY nasdsc
                            ";
                            $db->monta_combo("nasid", $sql, $habil, "Selecione...", '', '', '', '', 'N', 'nasid');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" align='left' bgcolor="#CCCCCC"><b>Publicação</b></td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Número do DOU:</td>
                    <td><?= campo_texto('prtnumdiario', 'N', $habil, '', 15, 10, '', ''); ?></td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Número da Seção:</td>
                    <td><?= campo_texto('prtsecaodiario', 'N', $habil, '', 15, 10, '', ''); ?></td>
                </tr>

                <tr>
                    <td align='right' class="SubTituloDireita">Data do DOU:</td>
                    <td><?= campo_data('prtdtpublicdou', 'N', $habil, '', '', '', ''); ?></td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Número da Página:</td>
                    <td><?= campo_texto('prtpaginadiario', 'N', $habil, '', 15, 10, '', ''); ?></td>
                </tr>
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
                    <td>&nbsp;</td>
                </tr>
            </table>

            <? if (isset($_SESSION["academico"]["prtid"]) and $_SESSION["academico"]["prtid"] != "") { ?>

                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                    <tr style="background-color: #cccccc">
                        <td><b>Composição da Portaria</b></td>
                    </tr>
                </table>

                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                    <tr>
                        <td>
                            <br>
                            <input type="hidden" id="atualiza_ordem" name="atualiza_ordem" value="">

                            <table id="tabela_PartePrortarias" width="80%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                <thead>
                                    <tr id="cabecalho">
                                        <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ordem</strong></td>
                                        <td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ação</strong></td>
                                        <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Tipo</strong></td>
                                        <td width="80%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Conteúdo</strong></td>
                                    </tr>
                                </thead>

                                <?= montaTabela($disabled); ?>

                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <input type="hidden" id="listaPartePrortarias" name="listaPartePrortarias">
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <? if (!$arquivos_gerados && $habilitado) { ?>
                                <a href="#" onclick="javascript: inserirPartePrortaria();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">&nbsp;&nbsp;Inserir Novo</a>
                            <?php } ?>
                        </td>
                    </tr>
                </table>
            <? } ?>

            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
                    <td>
                        <input type="button" class="botao" name="btassociar" value="Gravar" onclick="javascript: salvarOrdenacao();" <? echo $disabled; ?>>
                        <?PHP
                            if(!$arquivos_gerados){
                                if(isset($_SESSION["academico"]["prtid"]) and $_SESSION["academico"]["prtid"] != ""){
                                    if($gerar_portaria){
                        ?>
                                        <input type="button" class="botao" name="btgerar" value="Gerar Portaria" onclick="gerarPortaria('<?= $_SESSION["academico"]["prtid"] ?>');" <? echo $disabled; ?>>
                        <?PHP
                                    }
                                }
                            }

                            if ($_SESSION["academico"]["tprnivel"] == 1) {
                        ?>
                            <input type="button" class="botao" name="btassociar" value="Voltar" onclick="javascript: window.open('academico.php?modulo=principal/listarPortarias&acao=C&orgid=1', '_top');" <? echo $disabled; ?>>

                        <? } else { ?>

                            <input type="button" class="botao" name="btassociar" value="Voltar" onclick="javascript: window.open('academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid=<?= $_SESSION["academico"]["prtidautprov"]; ?>', '_top');" <? echo $disabled; ?>>

                        <? } ?>
                    </td>
                </tr>
            </table>
            <br>

            <? if(isset($_SESSION["academico"]["prtid"]) and $_SESSION["academico"]["prtid"] != ""){ ?>

                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                    <tr style="background-color: #cccccc">
                        <td>
                            <b>
                                <?PHP
                                    if ($_SESSION["academico"]["tprnivel"] == ACA_TPORTARIA_CONCURSO){
                                        echo "Autorizações de Provimento";
                                    }else{
                                        echo "Autorização de Concurso";
                                    }
                                ?>
                            </b>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <?PHP
                                if ($_SESSION["academico"]["prtid"]) {

                                    if ($habilitado) {
                                        $btexcluir = "<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"apagar_provimetno(' || p.prtid || ');\" title=\"Excluir\">";
                                    } else {
                                        $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
                                    }
                                    
                                    if ($_SESSION["academico"]["tprnivel"] == 1) {
                                        $sql = "
                                            SELECT '<center> 
                                                    <a href=\"academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid=' || p.prtid || '&prtidautprov=' || p.prtidautprov || '\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>
                                                    {$btexcluir}
                                                    </center>',
                                                    '<center>' ||TO_CHAR(p.prtdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
                                                    '<center>' || tp.tprdsc  ||'</center>' as tipoportaria,
                                                    '<center>' || p.prtid ||'</center>' AS cod,
                                                    '<center>' || p.prtnumero ||'</center>' as numero,
                                                    '<center>' ||  u.usunome ||'</center>' as nome
                                                    
                                            FROM academico.portarias AS p
                                            
                                            INNER JOIN academico.orgao o on o.orgid = p.orgid
                                            INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
                                            INNER JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                                            
                                            WHERE p.prtstatus = 'A' AND p.prtidautprov = {$_SESSION["academico"]["prtid"]}
                                        ";
                                        $cabecalho = array("Ação", "Data Inclusão", "Tipo de Portaria", "Nº de Controle", "Nº Portaria", "Nome");
                                        
                                    } else {

                                        $sql = "
                                            SELECT  '<center>' ||TO_CHAR(p.prtdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
                                                    '<center>' ||'<a href=academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid='|| p.prtid ||'>
                                                    ' || tp.tprdsc ||'</a>' ||'</center>' as tipoportaria,
                                                    '<center>' ||p.prtid ||'</center>'  AS cod,
                                                    '<center>' ||p.prtnumero ||'</center>' as numero,
                                                    '<center>' ||u.usunome  ||'</center>' as nome
                                                    
                                            FROM academico.portarias AS p
                                            
                                            INNER JOIN academico.orgao o on o.orgid = p.orgid
                                            INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
                                            INNER JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                                            
                                            WHERE p.prtstatus = 'A'AND p.prtid = {$_SESSION["academico"]["prtidautprov"]}
                                                
                                            ORDER BY p.prtdtportaria DESC
                                        ";
                                        $cabecalho = array( "Data Inclusão", "Tipo de Portaria", "Nº de Controle", "Nº Portaria", "Nome" );
                                    }
                                    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '');
                                }
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <? if ($_SESSION["academico"]["tprnivel"] == 1 && $habilitado) { ?>
                                <a href="#" onclick="javascript: inserirAutProvimento();">
                                    <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">&nbsp;&nbsp;Inserir Autorização de Provimentos
                                </a>
                        </td>
                            <? } ?>
                    </tr>
                </table>
            
            <?  } 
            
                if ($_SESSION["academico"]["prtid"]) {
                    $sql = "
                        SELECT  '<center><a href=\"#\" onclick=\"javascript:excluirDocumento(\'EA\',' || arq.arqid || ',' || aqb.anxid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,
                                '<center>' || to_char(aqb.anxdtinclusao,'DD/MM/YYYY') ||'</center>' as anxdtinclusao,
                                '<center>' ||tpa.tpadsc||'</center>' as dsc,
                                '<center><a style=\"cursor: pointer; color: blue;\" onclick=\"javascript: downloadArquivo(\'DA\',' ||aqb.anxid|| ',' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a> </center>' as t1,
                                '<center>' ||arq.arqtamanho || ' '||'</center>' as tamanho ,
                                '<center>' ||arq.arqnome ||'</center>' as arqnome,
                                '<center>' ||u.usunome ||'</center>' as usunome
                        FROM public.arquivo arq
                        
			INNER JOIN academico.anexos aqb ON aqb.arqid = arq.arqid
			INNER JOIN academico.portarias pta ON pta.prtid = aqb.prtid
                        LEFT JOIN seguranca.usuario AS u ON u.usucpf = arq.usucpf
                        LEFT JOIN academico.tipoarquivo AS tpa ON tpa.tpaid= aqb.tpaid
                        
                        WHERE aqb.anxstatus = 'A' AND pta.prtid = {$_SESSION["academico"]["prtid"]} AND pta.prtano = '{$ano}'
                    ";
                    $cabecalho = array("Ação", "Data Inclusão", "Tipo Arquivo", "Nome Arquivo", "Tamanho (kbs)", "Descrição Arquivo", "Responsável" );
                    //$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
                }
    ?>


<script type="text/javascript">
    function inserirAutProvimento() {
        return windowOpen('academico.php?modulo=principal/cadportaria&acao=C&orgid=1&prgid=<?= $prgid; ?>&evento=N&tprnivel=2&prtidautprov=<?= $_SESSION["academico"]["prtid"]; ?>', '_top');
    }

    function inserir_inciso(artigo) {
        return windowOpen('academico.php?modulo=principal/cadpartes_portaria&acao=C&copidinciso=' + artigo + '&prtid=<?= $_SESSION["academico"]["prtid"]; ?>', 'partes', 'height=250,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes');
    }
    function editarPartePrortaria(copid, prtid, artigo) {
        return windowOpen('academico.php?modulo=principal/cadpartes_portaria&acao=C&copidinciso=' + artigo + '&evento=A&copid=' + copid + '&prtid=' + prtid, 'Partes', 'height=250,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes');
    }

    function inserirPartePrortaria() {
        try {
            linhas = parseInt(document.getElementById('tabela_PartePrortarias').rows.length);
            ordem = ordem = document.getElementById('tabela_PartePrortarias').rows[linhas - 1].id;
        } catch (err) {
        }
        windowOpen('academico.php?modulo=principal/cadpartes_portaria&acao=C&copidinciso=0&copordem=' + ordem + '&prtid=<?= $_SESSION["academico"]["prtid"]; ?>', 'Partes', 'height=250,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes');
    }
    function excluiItem(linha, copid) {
        if (confirm('Deseja realmente excluir o Artigo / Parágrafo?')) {
            windowOpen('academico.php?modulo=principal/cadpartes_portaria&acao=C&evento=E&copid=' + copid, 'cnt');
        }
    }

    function excluiItem_inciso(copid) {
        if (confirm('Deseja realmente excluir o Inciso / Parágrafo??')) {
            windowOpen('academico.php?modulo=principal/cadpartes_portaria&acao=C&evento=E&copid=' + copid, 'cnt');
        }
    }
    function troca_linhas(idTabela, acao, linha, ncolAfetada) {

        switch (acao) {
            case 'cima':
                linha1 = linha;
                linha2 = (linha - 1);
                break;
            case 'baixo':
                linha1 = linha;
                linha2 = (linha + 1);
                break;
        }
        if (linha2 != 0 && linha2 < (document.getElementById(idTabela).rows.length - 1)) {
            for (i = 1; i <= ncolAfetada; i++) {
                cel1 = document.getElementById(idTabela).rows[linha1].cells[i].innerHTML;
                cel2 = document.getElementById(idTabela).rows[linha2].cells[i].innerHTML;

                document.getElementById(idTabela).rows[linha1].cells[i].innerHTML = cel2;
                document.getElementById(idTabela).rows[linha2].cells[i].innerHTML = cel1;
            }
            cel1 = document.getElementById(idTabela).rows[linha1].cells[2].innerHTML;
            cel2 = document.getElementById(idTabela).rows[linha2].cells[2].innerHTML;
            if (cel1.substr(0, 6) == 'Artigo' && cel2.substr(0, 6) == 'Artigo') {
                // voltar o numero do artigo
                document.getElementById(idTabela).rows[linha1].cells[2].innerHTML = cel2;
                document.getElementById(idTabela).rows[linha2].cells[2].innerHTML = cel1;
            }
        }
    }

    function salvarOrdenacao() {

        try {
            atualiza_quant_artigos = document.getElementById('atualiza_quant_artigos').value;
            // Tabela de fora
            var tabela = 'tabela_PartePrortarias';
            linhas = parseInt(document.getElementById(tabela).rows.length);
            var id = 0;
            var ordem = 0;
            for (i = 1; i < linhas; i++) {
                ordem = document.getElementById(tabela).rows[i].id;
                id = document.getElementById(tabela).rows[i].cells[4].innerHTML;
                document.getElementById('atualiza_ordem').value = document.getElementById('atualiza_ordem').value + "," + id + "_" + ordem;
            }

            // Tabelas de dentro
            for (p = 0; p < atualiza_quant_artigos; p++) {
                var tabela = 'tabela_PartePrortarias_' + p;
                linhas = parseInt(document.getElementById(tabela).rows.length);
                var id = 0;
                var ordem = 0;
                for (i = 1; i < linhas; i++) {
                    ordem = document.getElementById(tabela).rows[i].id;
                    id = document.getElementById(tabela).rows[i].cells[4].innerHTML;
                    document.getElementById('atualiza_ordem').value = document.getElementById('atualiza_ordem').value + "," + id + "_" + ordem;
                }
            }
        }
        catch (err) {
        }

        // Salva
        submeter('I', '<?= $anxid ?>', '<?= $arqid ?>', '', '', '<?= $_REQUEST['prtidautprov'] ?>');
    }

    function submeter(evento, campo, lanid, totais_autorizado, totais_tipoEP, prtidautprov) {
        if (validar()) {
            document.getElementById('evento').value = evento;
            document.getElementById('prtidautprov').value = prtidautprov;
            document.formulario.submit();
        }
    }

    function apagar_provimetno(prtid) {
        if (confirm('Deseja realmente excluir este Provimento?'))
            location.href = '/academico/academico.php?modulo=principal/cadportaria&acao=C&evento=E&prtid=' + prtid;
    }

    function novo() {
        document.getElementById('evento').value = '';
        document.getElementById('prtid').value = '';
        document.formulario.submit();
    }

    function novoArq() {
        document.getElementById('evento').value = '';
        document.getElementById('arqid').value = '';
        document.formulario.submit();
    }

    function limpar() {
        document.getElementById('evento').value = '';
        document.formulario.prtnumero.value = '';
        document.formulario.prtdsc.value = '';
        document.formulario.prtnumdiario.value = '';
        document.formulario.prtdtpublicdou.value = '';
    }

    function limparArq() {
        document.getElementById('evento').value = '';
        document.formulario.arquivo.value = '';
        document.formulario.tpaid.value = '';
        document.formulario.arqdescricao.value = '';
    }

    function validar() {
        var msg = "";
        if (document.formulario.prgid.value == '') {
            msg += "O preenchimento do campo \"Programa\" é obrigatório.\n";
        }
        
        if (msg != "") {
            alert(msg);
            return false;
        } else
            return true;
    }

    function validarArquivo() {

        if (document.formulario.arquivo.value == "") {
            alert("Campo Arquivo obrigatório.");
            document.formulario.arquivo.focus();
            return false;
        }
        if (document.formulario.tpaid.value == "") {
            alert("Campo Tipo obrigatório.");
            document.formulario.tpaid.focus();
            return false;
        }
        if (document.formulario.arqdescricao.value == "") {
            alert("Campo Descrição obrigatório.");
            document.formulario.arqdescricao.focus();
            return false;
        }
        return true;
    }

    function excluirDocumento(evento, arqid, anxid) {
        if (confirm("Deseja realmente excluir este documento ?")) {
            document.getElementById('evento').value = 'EA';
            document.getElementById('evento_arquivo').value = 'EA';
            document.getElementById('arqid').value = arqid;
            document.getElementById('anxid').value = anxid;
            document.formulario.submit();
        }
    }
    function downloadArquivo(evento, anxid, arqid) {
        document.getElementById('evento').value = '';
        document.getElementById('evento_arquivo').value = evento;
        document.getElementById('anxid').value = anxid;
        document.getElementById('arqid').value = arqid;
        document.formulario.submit();
    }
    function gerarPortaria(prtid) {
        window.open(
                '?modulo=relatorio/reportAcademico&acao=C&prtid=' + prtid,
                'Portaria',
                'width=600,height=800,scrollbars=yes,scrolling=yes,resizebled=yes'
                );
        document.getElementById('msg_nao_mostrar').value = '1';
        salvarOrdenacao();
    }
</script>

