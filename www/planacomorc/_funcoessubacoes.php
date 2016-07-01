<?php
/**
 * Fun?ões de apoio ao gerenciamento de suba?ões.
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 * @version $Id: _funcoessubacoes.php 92616 2015-01-13 16:45:53Z lindalbertofilho $
 */

/**
 *
 * @global type $db
 * @param type $dados
 */
function carregarSubacao($dados) {
    global $db;

    $sql = "SELECT * FROM planacomorc.subacao
			WHERE id_subacao='" . $dados['id_subacao'] . "'";
    $subacao = $db->pegaLinha($sql,d);
    $_SESSION['planacomorc']['id_subacao'] = $subacao['id_subacao'];
    $_SESSION['planacomorc']['sbacod'] = $subacao['sbacod'];

    $al = array(
        "location" => "planacomorc.php?modulo=principal/subacoes/gerenciarsubacoes&acao=A&aba=monitorarsubacao"
                        . ($dados['id_periodo_referencia']?"&id_periodo_referencia={$dados['id_periodo_referencia']}":'')
    );
    alertlocation($al);
}

function carregarSubacao2($dados) {
    global $db;

    $sql = "SELECT * FROM planacomorc.subacao
			WHERE id_subacao='" . $dados['id_subacao'] . "'";
    $subacao = $db->pegaLinha($sql);
    $_SESSION['planacomorc']['id_subacao'] = $subacao['id_subacao'];
    $_SESSION['planacomorc']['sbacod'] = $subacao['sbacod'];
    #$apenasListar = true;
    require_once(APPRAIZ . 'planacomorc/modulos/principal/subacoes/monitorarsubacao.inc');
    exit();
}

function montaAbasSisopSubAcoes($abaativa = null) {
    global $db;

    $menu[] = array("id" => 1, "descricao" => "Lista Subações", "link" => "/planacomorc/planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A");;
    $menu[] = array("id" => 2, "descricao" => "Subações ( " . $_SESSION['exercicio'] . " )", "link" => "/planacomorc/planacomorc.php?modulo=principal/subacoes/gerenciarsubacoes&acao=A&aba=monitorarsubacao");

    echo montarAbasArray($menu, $abaativa);
}

function montarCabecalhoSubacao($dados) {
    global $db;

    if (!$dados['id_subacao']) {
        echo <<<JS
<script type="text/javascript" language="JavaScript">
window.location = "planacomorc.php?modulo=inicio&acao=C"
</script>
JS;
        die();
    }


    $sql = "SELECT * FROM planacomorc.subacao
			WHERE id_subacao='" . $dados['id_subacao'] . "'";
    $subacao = $db->pegaLinha($sql);

    $cabecalho .= '<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
    $cabecalho .= '<tr>';
    $cabecalho .= '<td class="SubTituloDireita" width=17%>Subação :</td>';
    $cabecalho .= '<td colspan="5">' . $subacao['codigo'] . ' - ' . $subacao['titulo'] . '</td>';
    $cabecalho .= '</tr>';
    $cabecalho .= '</table>';

    return $cabecalho;
}

function consultarAcao($dados) {
    global $db;

    $sql = "SELECT
				a.acaexercicio,
				a.acatitulo,
				a.acadescricao,
				a.acabaselegal,
				(SELECT esfdsc FROM public.esfera WHERE esfcod=a.esfcod::integer) as esfera,
			  	(SELECT protitulo FROM planacomorc.programa WHERE procod=a.procod) as programa,
			  	(SELECT descricaoabreviada FROM planacomorc.funcao WHERE codigofuncao=a.acacodigofuncao AND exercicio='" . $_SESSION['exercicio'] . "') as funcao,
			  	(SELECT titulo FROM planacomorc.iniciativa WHERE codigoiniciativa=a.acacodigoiniciativa AND codigoobjetivo=a.acacodigoobjetivo AND codigoorgao=a.orgcod AND exercicio='" . $_SESSION['exercicio'] . "') as iniciativa,
			  	a.acacodigomomento,
			  	(SELECT enunciado FROM planacomorc.objetivo WHERE codigomomento=a.acacodigomomento::integer AND codigoobjetivo=a.acacodigoobjetivo AND codigoorgao=a.orgcod AND codigoprograma=a.procod AND exercicio='" . $_SESSION['exercicio'] . "') as objetivo,
			  	(SELECT descricao FROM planacomorc.produto WHERE codigoproduto=a.procod::integer) as produto,
			  	(SELECT descricaoabreviada FROM planacomorc.subfuncao WHERE codigosubfuncao=a.acacodigosubfuncao AND codigofuncao=a.acacodigofuncao AND exercicio='" . $_SESSION['exercicio'] . "') as subfuncao,
			  	a.acacodigotipoacao,
			  	a.acacodigotipoinclusaoacao,
			  	(SELECT descricao FROM planacomorc.unidademedida WHERE codigounidademedida=a.acacodigounidademedida) as unidademedida,
			  	a.acadetalhamentoimplementacao,
			  	a.acaespecificacaoproduto,
			  	a.acafinalidade,
			  	a.acaformaacompanhamento,
			  	a.acaidentificacaosazonalidade,
			  	a.acainsumosutilizados,
			  	a.acasndescentralizada,
			  	a.acasndireta,
			  	a.acaativo,
			  	a.acasnlinhacredito,
			  	a.acasntransferenciaobrigatoria,
			  	a.acasntransferenciavoluntaria,
			  	a.acaunidaderesponsavel
			FROM planacomorc.acao a
			WHERE a.acaidentificadorunicosiop='" . $dados['acaidentificadorunicosiop'] . "'";

    $acao = $db->pegaLinha($sql);

    $html .= '<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Exercício :</td><td>' . $acao['acaexercicio'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Título :</td><td>' . $acao['acatitulo'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Descrição :</td><td>' . $acao['acadescricao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Base legal :</td><td>' . $acao['acabaselegal'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Esfera :</td><td>' . $acao['esfera'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Programa :</td><td>' . $acao['programa'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Função :</td><td>' . $acao['funcao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Iniciativa :</td><td>' . $acao['iniciativa'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Momento :</td><td>' . $acao['acacodigomomento'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Objetivo :</td><td>' . $acao['objetivo'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Produto :</td><td>' . $acao['produto'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Subfunção :</td><td>' . $acao['subfuncao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Tipo ação :</td><td>' . $acao['acacodigotipoacao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Tipo inclusão ação :</td><td>' . $acao['acacodigotipoinclusaoacao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Unidade medida :</td><td>' . $acao['unidademedida'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Detalhamento implementação :</td><td>' . $acao['acadetalhamentoimplementacao'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Especificação do produto :</td><td>' . $acao['acaespecificacaoproduto'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Finalidade :</td><td>' . $acao['acafinalidade'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Forma acompanhamento :</td><td>' . $acao['acaformaacompanhamento'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Identificação sazonalidade :</td><td>' . $acao['acaidentificacaosazonalidade'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Insumos utilizados :</td><td>' . $acao['acainsumosutilizados'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Descentralizadas :</td><td>' . $acao['acasndescentralizada'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Direta :</td><td>' . $acao['acasndireta'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Ativo :</td><td>' . $acao['acaativo'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Linha de crédito :</td><td>' . $acao['acasnlinhacredito'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Transferência Obrigatória :</td><td>' . $acao['acasntransferenciaobrigatoria'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Transferência Voluntária :</td><td>' . $acao['acasntransferenciavoluntaria'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloDireita" width=25%>Unidade responsável :</td><td>' . $acao['acaunidaderesponsavel'] . '</td></tr>';
    $html .= '<tr><td class="SubTituloCentro" colspan="2"><input type=button name=fechar value=Fechar onclick="jQuery(\'#modalAcao\').dialog(\'close\');"></td></tr>';

    $html .= '</table>';

    echo $html;
}

function gerenciarMonitorInterno($dados) {
    global $db;

    $sql = "SELECT moncpf, monnome, SUBSTR(monfone,1,2) as monfoneddd, SUBSTR(monfone,3) as monfone, monemail FROM planacomorc.monitorinterno WHERE orgcod='" . $dados['orgcod'] . "' AND monano='" . $_SESSION['exercicio'] . "'";
    $monitorinterno = $db->pegaLinha($sql);

    if ($monitorinterno)
        extract($monitorinterno);

    echo "<form method=\"post\" id=\"frm_monitorinterno\" name=\"frm_monitorinterno\" action=\"planacomorc.php?modulo=principal/acoes/listaunidades&acao=A\">";
    echo "<input type=hidden name=requisicao value=gravarMonitorInterno>";
    echo "<input type=\"hidden\" name=\"orgcod\" id=\"orgcod\" value=" . $dados['orgcod'] . ">";

    echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
    echo "<tr>";
    echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir</td>";
    echo "</tr>";
    echo "<tr>";
    echo "<td class=\"SubTituloDireita\" width=\"40%\">CPF:</td>";
    echo "<td>" . campo_texto('moncpf', "S", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="moncpf"', '', mascaraglobal($moncpf, "###.###.###-##"), 'if(this.value.length==14){carregaUsuario();}else{this.value=\'\';}') . "</td>";
    echo "</tr>";
    echo "<tr>";
    echo "<td class=\"SubTituloDireita\" width=\"40%\">Nome:</td>";
    echo "<td>" . campo_texto('monnome', "S", "N", "Nome", 45, 180, "", "", '', '', 0, 'id="monnome"', '', $monnome) . "</td>";
    echo "</tr>";
    echo "<tr>";
    echo "<td class=\"SubTituloDireita\" width=\"40%\">Telefone:</td>";
    echo "<td>" . campo_texto('monfoneddd', "N", "S", "Telefone", 2, 3, "##", "", '', '', 0, 'id="monfoneddd"', '', $monfoneddd) . " " . campo_texto('monfone', "S", "S", "Telefone", 9, 10, "########", "", '', '', 0, 'id="monfone"', '', $monfone) . "</td>";
    echo "</tr>";
    echo "<tr>";
    echo "<td class=\"SubTituloDireita\" width=\"40%\">Email:</td>";
    echo "<td>" . campo_texto('monemail', "S", "S", "Email", 45, 50, "", "", '', '', 0, 'id="monemail"', '', $monemail) . "</td>";
    echo "</tr>";
    echo "<tr>";
    echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarMonitor();> <input type=button value=Cancelar onclick=\"jQuery('#modalMonitorInterno').dialog('close');\"></td>";
    echo "</tr>";
    echo "</table>";
    echo "</form>";
}

function gravarMonitorInterno($dados) {
    global $db;

    $sql = "SELECT monid FROM planacomorc.monitorinterno WHERE orgcod='" . $dados['orgcod'] . "' AND monano='" . $_SESSION['exercicio'] . "'";
    $monid = $db->pegaUm($sql);

    if ($monid) {
        $sql = "UPDATE planacomorc.monitorinterno
				SET moncpf='" . str_replace(array(".", "-"), array("", ""), $dados['moncpf']) . "',
					monnome='" . $dados['monnome'] . "',
					monfone='" . $dados['monfone'] . "',
					monemail='" . $dados['monemail'] . "'
				WHERE monid='" . $monid . "'";

        $db->executar($sql);
    } else {
        $sql = "INSERT INTO planacomorc.monitorinterno(
	            moncpf, monnome, monfone, monemail, orgcod, monano)
			    VALUES ('" . str_replace(array(".", "-"), array("", ""), $dados['moncpf']) . "', '" . $dados['monnome'] . "', '" . $dados['monfone'] . "', '" . $dados['monemail'] . "', '" . $dados['orgcod'] . "', '" . $_SESSION['exercicio'] . "');";

        $db->executar($sql);
    }

    $db->commit();
}

function listaSubacoes($dados) {
    global $db;
    $perfis = pegaPerfilGeral();
    if (1 != $_SESSION['superuser']) {
        if (in_array(PFL_COORDENADORSUBACAO, $perfis)) {
            $whpfl[] = "EXISTS (SELECT 1
                                      FROM planacomorc.usuarioresponsabilidade urb
                                      WHERE s.id_subacao = urb.id_subacao
                                        AND urb.usucpf = '{$_SESSION['usucpf']}'
                                        AND urb.pflcod = " . PFL_COORDENADORSUBACAO . ")";
        }
    }
    $sql = " SELECT DISTINCT
                    '<img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"gerenciarSubacao(\''||apr.id_subacao||'\');\">' as acao,
					' '||s.codigo||' - '||s.titulo AS descricao,
					CASE WHEN esdid IS NULL THEN 1 ELSE 0 END AS naoiniciado,
               				CASE WHEN esdid = 969 THEN 1 ELSE 0 END AS empreenchimento,
	               			CASE WHEN esdid = 970 THEN 1 ELSE 0 END AS emanalise,
	               			CASE WHEN esdid = 971 THEN 1 ELSE 0 END AS aprovado
					FROM
					   planacomorc.snapshot_dotacao_subacao apr
					INNER JOIN planacomorc.subacao s USING(id_subacao)
					LEFT JOIN planacomorc.acompanhamento_subacao aco  ON (apr.id_subacao = aco.id_subacao AND aco.id_periodo_referencia = {$dados['id_periodo_referencia']} AND apr.id_ptres = aco.id_ptres )
					LEFT JOIN workflow.documento doc ON doc.docid = aco.docid
					WHERE
					s.st_ativo='A'
                    AND s.id_exercicio='" . $_SESSION['exercicio'] . "' " . (($whpfl) ? "
                    AND " . implode(" AND ", $whpfl) : "") . "
					AND s.id_exercicio = {$_SESSION['exercicio']}
					AND apr.id_periodo_referencia =" .$dados['id_periodo_referencia'] ;

    $cabecalho = array("&nbsp;","Unidade", "Não iniciado", "Em preenchimento", "Em Análise", "Aprovado");
    $db->monta_lista($sql, $cabecalho, 250, 5, 'S', 'center', $par2);
}

function listaSubacoes_bootstrap($dados) {

    global $db;
    $perfis = pegaPerfilGeral();
    if (1 != $_SESSION['superuser']) {
        if (in_array(PFL_COORDENADORSUBACAO, $perfis)) {
                $whpfl[] = "EXISTS (SELECT 1
                FROM planacomorc.usuarioresponsabilidade urb
                WHERE s.id_subacao = urb.id_subacao
                AND urb.usucpf = '{$_SESSION['usucpf']}'
                AND urb.pflcod = " . PFL_COORDENADORSUBACAO . ")";
        }
    }

    if($_SESSION['exercicio']=='2013'){
        $sql = "
            SELECT
                DISTINCT apr.id_subacao as acao,
                ' '||s.codigo||' - '||s.titulo AS descricao,
                0 AS naoiniciado,
                0 AS empreenchimento,
                0 AS emanalise,
                0 AS emcorrecao,
                0 AS aprovado
            FROM planacomorc.snapshot_dotacao_subacao apr
            INNER JOIN planacomorc.subacao s USING(id_subacao)
            LEFT JOIN planacomorc.acompanhamento_subacao aco ON (apr.id_subacao = aco.id_subacao AND aco.id_periodo_referencia = {$dados['id_periodo_referencia']} AND apr.id_ptres = aco.id_ptres )
            WHERE s.st_ativo='A'
                AND s.id_exercicio='" . $_SESSION['exercicio'] . "' " . (($whpfl) ? "
                AND " . implode(" AND ", $whpfl) : "") . "
                AND s.id_exercicio = {$_SESSION['exercicio']}
                AND apr.id_periodo_referencia =" .$dados['id_periodo_referencia'] ." ORDER BY 2
        ";
    } else {
        $sql = "
            SELECT
                DISTINCT apr.id_subacao as acao,
                ' '||s.codigo||' - '||s.titulo AS descricao,
                CASE WHEN COALESCE(doc.esdid) IS NULL THEN 1 ELSE 0 END AS naoiniciado,
                CASE WHEN COALESCE(doc.esdid) = 969 THEN 1 ELSE 0 END AS empreenchimento,
                CASE WHEN COALESCE(doc.esdid) = 970 THEN 1 ELSE 0 END AS emanalise,
                CASE WHEN COALESCE(doc.esdid) = 1285 THEN 1 ELSE 0 END AS emcorrecao,
                CASE WHEN COALESCE(doc.esdid) = 971 THEN 1 ELSE 0 END AS aprovado,
                SUM(apr.empenhado) as empenhado
            FROM planacomorc.snapshot_dotacao_subacao apr
            INNER JOIN planacomorc.subacao s USING(id_subacao)
            LEFT JOIN planacomorc.acompanhamento_subacao_tramitacao ast ON ast.id_subacao = s.id_subacao AND ast.id_periodo_referencia = {$dados['id_periodo_referencia']}
            LEFT JOIN workflow.documento doc ON ast.docid = doc.docid
            INNER JOIN planacomorc.ptres pt ON pt.id_ptres = apr.id_ptres
            INNER JOIN planacomorc.acao_programatica prg ON prg.id_acao_programatica = pt.id_acao_programatica
            INNER JOIN planacomorc.acao aca ON aca.id_acao = prg.id_acao
            WHERE s.st_ativo='A'
                AND s.id_exercicio='" . $_SESSION['exercicio'] . "' " . (($whpfl) ? "
                AND " . implode(" AND ", $whpfl) : "") . "
                AND apr.id_periodo_referencia =" .$dados['id_periodo_referencia'] ."
                --AND prg.id_exercicio = 2014
                AND empenhado > 0
            GROUP BY acao,s.codigo,s.titulo,doc.esdid
            ORDER BY 2
        ";
    }

    $cabecalho = array("Unidade", "Não iniciado", "Em preenchimento", "Em Análise","Em Correção", "Aprovado");
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->addAcao('edit', 'gerenciarSubacao');
    $listagem->setQuery($sql);
    $listagem->esconderColunas('empenhado');

    $camposTotalizados = array('naoiniciado', 'empreenchimento', 'emanalise', 'emcorrecao', 'aprovado');

    $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda')
        ->addCallbackDeCampo($camposTotalizados, 'formatarTotais');
    $listagem->setCabecalho($cabecalho);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $camposTotalizados);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

/**
 * Formata os campos do acompanhamento de subações e seus totais.
 *
 * @param integer $valor 1) Indica o estado atual da linha; 0) Indica os demais estados da linha.
 * @param array|null $dados Array com os demais dados da linha. Nullo no caso de execução em um totalizador.
 * @return string
 */
function formatarTotais($valor, $dados = null)
{
    if (is_null($dados)) {
        return mascaraNumero($valor);
    }
    switch ($valor) {
        case '1':
            return '<p style="text-align:center"><span style="color:green" class="glyphicon glyphicon-ok"></span></p>';
            // no break
        case '0':
            return '<p style="text-align:center"><span style="color:grey" class="glyphicon glyphicon-minus"></span></p>';
            // no break
    }
}

function gravarAcompanhamentoSubAcao($dados) {
    global $db;

    $comentario_ptres = $dados['comentario_ptres'];
    if ($dados['fisico_convertido_acao'] && is_array($dados['fisico_convertido_acao'])) {
        foreach ($dados['fisico_convertido_acao'] as $id_ptres => $fisico_convertido_acao) {
            $sql = <<<DML
SELECT id_acompanhamento_subacao
  FROM planacomorc.acompanhamento_subacao
  WHERE id_subacao = '{$dados['id_subacao']}'
    AND id_ptres = '{$id_ptres}'
    AND id_periodo_referencia = '{$dados['id_periodo_referencia']}'
DML;

            $id_acompanhamento_subacao = $db->pegaUm($sql);

            /* Cria um DOCID para o Workflow */
            criaDocumentoSubacao($id_acompanhamento_subacao);

            if (trim($fisico_convertido_acao) === '') {
                $wh_fisico_convertido_acao = 'NULL';
            } else {
                $wh_fisico_convertido_acao = $fisico_convertido_acao;
            }
            $wh_fisico_convertido_acao = str_replace('.', '', $wh_fisico_convertido_acao);
            if ($id_acompanhamento_subacao) {
                $sql = "update planacomorc.acompanhamento_subacao set
        		          fisico_convertido_acao = $wh_fisico_convertido_acao,
        		          comentario_ptres = '{$comentario_ptres[$id_ptres]}'
        		        where id_acompanhamento_subacao = $id_acompanhamento_subacao;";

                $db->executar($sql);
            } else {
                $wh_fisico_convertido_acao = str_replace('.', '', $wh_fisico_convertido_acao);
                $sql = "INSERT INTO planacomorc.acompanhamento_subacao(id_periodo_referencia, id_subacao, id_ptres,fisico_convertido_acao,comentario_ptres)
				        VALUES ('" . $dados['id_periodo_referencia'] . "', '" . $dados['id_subacao'] . "', '$id_ptres', $wh_fisico_convertido_acao,'{$comentario_ptres[$id_ptres]}')
                        RETURNING id_acompanhamento_subacao;";
                $id_acompanhamento_subacao = $db->pegaUm($sql);
            }
            // -- Gravando o produto da suba??o
            if (isset($dados['produto_spo']) && is_array($dados['produto_spo'])) {
                foreach ($dados['produto_spo'] as $id_produto_spo => $produto_spo) {
                    $sql = <<<DML
SELECT id_acompanhamento_subacao_produto_spo
  FROM planacomorc.acompanhamento_subacao_produto_spo
  WHERE id_produto_spo = {$id_produto_spo}
    AND id_acompanhamento_subacao = {$id_acompanhamento_subacao}
    AND id_periodo_referencia = {$dados['id_periodo_referencia']}
DML;
                    $id_acompanhamento_subacao_produto_spo = $db->pegaUm($sql);
                    if (empty($produto_spo[$id_ptres])) {
                        $produto_spo[$id_ptres] = 0;
                    }
                    if ($id_acompanhamento_subacao_produto_spo) {
                        $sql = <<<DML
UPDATE planacomorc.acompanhamento_subacao_produto_spo
  SET valor_fisico = {$produto_spo[$id_ptres]}
  WHERE id_acompanhamento_subacao_produto_spo = {$id_acompanhamento_subacao_produto_spo}
DML;
                    } else {
                        $sql = <<<DML
INSERT INTO planacomorc.acompanhamento_subacao_produto_spo(id_acompanhamento_subacao, id_produto_spo, valor_fisico, id_periodo_referencia)
  VALUES({$id_acompanhamento_subacao}, {$id_produto_spo}, {$produto_spo[$id_ptres]}, {$dados['id_periodo_referencia']})
DML;
                    }
                    $db->executar($sql);
                }
            }
        }
    }
    //ver("Último id acompanhamento subação passado: ".$id_acompanhamento_subacao,d);
    gravarQuestionarioSubacao($id_acompanhamento_subacao, $dados);

    $db->commit();

    $al = array(
        "alert" => "Monitoramento gravado com sucesso",
        "location" => "planacomorc.php?modulo=principal/subacoes/gerenciarsubacoes&acao=A&aba=monitorarsubacao"
            . "&id_subacao={$dados['id_subacao']}&id_periodo_referencia={$dados['id_periodo_referencia']}"
    );
    alertlocation($al);
}

function gravarQuestionarioSubacao($idAcompSubacao, $dados)
{
    global $db;

    // -- Limpando os dados previamente inseridos
    $sql = <<<DML
DELETE
  FROM planacomorc.monsubacaquestionarioresposta
  WHERE id_acompanhamento_subacao = '{$idAcompSubacao}'
DML;

    $db->executar($sql);

    /* Grava as Respostas dos Question?rios */
    if ($dados['idperguntas']) {
        foreach ($dados['idperguntas'] as $mqpid) {
            $sql = <<<DML
INSERT INTO planacomorc.monsubacaquestionarioresposta(id_acompanhamento_subacao,mqpid, mqrresposta, dtcriacao, usucpf,id_subacao)
  VALUES ({$idAcompSubacao},'{$mqpid}', '{$dados['resposta_' . $mqpid]}', NOW(), '{$_SESSION['usucpf']}',{$dados['id_subacao']})
DML;

            $db->executar($sql);
        }
    }
}

function diagnosticoMonitoramentoAcao($dados) {
    global $db;

    $sql = "SELECT
			'<center><img src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" onclick=\"detalharDiagnosticoMonitoramentoAcao(\''||o.codigo||'\', \'" . $dados['id_periodo_referencia'] . "\', this);\" title=\"mais\"></center>' as acao,
			orgcod || ' - ' || orgdescricao as descricao,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acao c
			LEFT JOIN planacomorc.acompanhamentoacao a ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "'
			WHERE a.docid IS NULL AND c.orgcod=o.codigo)" : "0") . " as naoiniciado,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acompanhamentoacao a
			INNER JOIN planacomorc.acao c ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop
			INNER JOIN workflow.documento d ON d.docid = a.docid
			WHERE d.esdid=" . ESD_EMELABORACAO . " AND c.orgcod=o.codigo AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "')" : "0") . " as emelaboracao,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acompanhamentoacao a
			INNER JOIN planacomorc.acao c ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop
			INNER JOIN workflow.documento d ON d.docid = a.docid
			WHERE d.esdid=" . ESD_EMVALIDACAO . " AND c.orgcod=o.codigo AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "')" : "0") . " as emvalidacao,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acompanhamentoacao a
			INNER JOIN planacomorc.acao c ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop
			INNER JOIN workflow.documento d ON d.docid = a.docid
			WHERE d.esdid=" . ESD_EMAPROVACAO . " AND c.orgcod=o.codigo AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "')" : "0") . " as emaprovacao,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acompanhamentoacao a
			INNER JOIN planacomorc.acao c ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop
			INNER JOIN workflow.documento d ON d.docid = a.docid
			WHERE d.esdid=" . ESD_FINALIZADO . " AND c.orgcod=o.codigo AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "')" : "0") . " as finalizado,
			" . (($dados['id_periodo_referencia']) ? "(SELECT COUNT(*) FROM planacomorc.acompanhamentoacao a
			INNER JOIN planacomorc.acao c ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop
			INNER JOIN workflow.documento d ON d.docid = a.docid
			WHERE d.esdid=" . ESD_ENVIADOSIOP . " AND c.orgcod=o.codigo AND a.id_periodo_referencia='" . $dados['id_periodo_referencia'] . "')" : "0") . " as enviadosiop
			FROM planacomorc.orgao o
			WHERE orgcod like '26%' AND orgtipoorgao='U' ORDER BY orgcod";


    $cabecalho = array("&nbsp;", "Unidade", "Não iniciado", "Em preenchimento", "Em validação", "Em aprovação", "Finalizado", "Enviado para SIOP");
    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
}

function detalharDiagnosticoMonitoramentoAcao($dados) {
    global $db;

    $sql = "SELECT
			'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"monitorarAcao('||c.acaidentificadorunicosiop||',\''||l.loccod||'\')\"> '||c.acacod||'.'||l.loccod||' - '||l.locdescricao as descricao,
			CASE WHEN d.esdid IS NULL THEN 1 ELSE 0 END as naoiniciado,
			CASE WHEN d.esdid=683 THEN 1 ELSE 0 END as emelaboracao,
			CASE WHEN d.esdid=684 THEN 1 ELSE 0 END as emvalidacao,
			CASE WHEN d.esdid=685 THEN 1 ELSE 0 END as emaprovacao,
			CASE WHEN d.esdid=686 THEN 1 ELSE 0 END as enviadosiop
			FROM planacomorc.acao c
			LEFT JOIN planacomorc.acompanhamentoacao a ON c.acaidentificadorunicosiop = a.acaidentificadorunicosiop AND a.id_periodo_referencia={$dados['id_periodo_referencia']}
			LEFT JOIN planacomorc.localizador l ON c.acaidentificadorunicosiop = l.acaidentificadorunicosiop
			LEFT JOIN workflow.documento d ON d.docid = a.docid
			WHERE c.orgcod='" . $dados['orgcod'] . "' ";

    $cabecalho = array("&nbsp;", "N?o iniciado", "Em elabora??o", "Em preenchimento", "Em valida??o", "Enviado para SIOP");
    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
}

/*
 * Lista de Suba?ões para MANTER
 */

function listaSubacoesManter($dados, $apenasObrigatorias = "") {
    global $db;
    if($apenasObrigatorias == 'n'){
        if($dados){
            $params['unicod']=$dados;
       $sql = retornaConsultaSubacao($params, 'n');
        }else{
           $sql = retornaConsultaSubacao(array(), 'n');
        }
    }else{
      $sql = retornaConsultaSubacao(array());
    }

    $cabecalho = array(
        "",
        "Código",
        "Subação",
        "Unidade Orçamentária",
        "Orçamento Atual (R$)",
        "Detalhado em PI (R$)",
        "Empenhado (R$)",
        "Não Detalhado em PI (R$)",
        "Não Empenhado (R$)"
    );
    //$db->monta_lista_ordenaGROUPBY($sql, $cabecalho, 250, 5, 'S', 'center');
    return $sql;
}

/*
 * Lista de Subações para MANTER
*/

function listaSubacoesManter_bootstrap($dados, $apenasObrigatorias = "") {
    global $db;
    if($apenasObrigatorias == 'n'){
        if($dados){
            $params['unicod']=$dados;
            $sql = retornaConsultaSubacao_bootstrap($params, 'n');
        }else{
            $sql = retornaConsultaSubacao_bootstrap(array(), 'n');
        }
    }else{
        if($dados['caixaAzul'] == 'S'){
            $params['caixaAzul'] = 'S';
        }else{
            $params['caixaAzul'] = 'N';
        }
        $params['SELECT'] = "
            SELECT DISTINCT
                sbaid,
                codigo as cod,
                sbatitulo,
                dotacao,
                SUM(empenhado) AS empenhado,
                SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado,
                CASE
                    WHEN SUM(CAST(detalhado_pi AS NUMERIC)) > 0
                    THEN 0
                ELSE 1
                END as delete
            ";
        $sql = retornaConsultaSubacao_bootstrap($params,'S');
    }
    $cabecalho = array(
        "Código",
        "Subação",
        "Orçamento Atual (R$)",
        "Empenhado (R$)",
        "Não Empenhado (R$)"
    );

    require_once APPRAIZ . 'includes/library/simec/Listagem.php';
    include_once APPRAIZ .'includes/funcoesspo.php';

    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $list->setCabecalho($cabecalho);
    $list->esconderColuna('delete');
    $list->setAcoes(array('edit'=>'alterarSubacao','delete'=> array('func' =>'removerSubacao','extra-params'=> array('cod')),'view'=>'detalheSubacao'));
    #ver($sql);
    $list->setQuery($sql);
    $list->setAcaoComoCondicional('delete', array(array('campo' => 'delete', 'valor' => '0', 'op' => 'diferente')));
    $list->addCallbackDeCampo(array('sbatitulo'), 'alinhaParaEsquerda');
    $list->addCallbackDeCampo(array('dotacao','detalhado_pi','empenhado','saldo_nao_detalhado','saldo_nao_empenhado'),'mascaraMoeda');
    $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $list->turnOnPesquisator();
    $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

function salvarSubacao($dados) {
    global $db;

    if ('I' == $dados['evento']) { // -- Insert
        $sqlNovaSubacao = <<<DML
INSERT INTO monitora.pi_subacao(sbadsc, sbasigla, sbatitulo, sbacod, usucpf, sbaobras, pieid, pigid, sbaano, sbasituacao)
  VALUES('%s', '%s', '%s', '%s', '%s', '%s', %d, %d, '%s', 'A')
  RETURNING sbaid
DML;
        #ver($sqlNovaSubacao,d);
        $sbacod = geraSbacod($dados['pieid'], $dados['pigid']);
        $dados['sbaobras'] = $dados['sbaobras'] ? $dados['sbaobras'] : 'F';
        $stmtNovaSubacao = sprintf(
                $sqlNovaSubacao, addslashes(trim($dados['sbadsc'])), addslashes(trim($dados['sbasigla'])), addslashes(trim($dados['sbatitulo'])), $sbacod, $_SESSION['usucpf'], $dados['sbaobras'], trim($dados['pieid']), trim($dados['pigid']), $_SESSION['exercicio']
        );
        #ver($stmtNovaSubacao,d);
        $sbaid = $db->pegaum($stmtNovaSubacao);
    } else { // -- Update - $dados['evento']
        if( $dados['sbaobras'] == '' ||  $dados['sbaobras'] == null){
            $sbaobras = 'F';
        }else{
            $sbaobras = $dados['sbaobras'];
        }
        $sqlUpdate = <<<DML
UPDATE monitora.pi_subacao
  SET sbadsc = '%s',
      sbasigla = '%s',
      sbatitulo = '%s',
      sbaobras = '%s'
  WHERE sbaid = {$dados['sbaid']}
DML;
        $stmtUpdate = sprintf(
                $sqlUpdate, addslashes(trim($dados['sbadsc'])), addslashes(trim($dados['sbasigla'])), addslashes(trim($dados['sbatitulo'])), $sbaobras
        );
        if ($db->executar($stmtUpdate)) {
            $sbaid = $dados['sbaid'];
        }
    }

    // -- Se inserir a nova suba??o
    if (!empty($sbaid)) {
        // -- Apagando as tabelas associativas quando for um update
        if ('U' == $dados['evento']) {
            deletaAssociativaSubacao('pi_subacaounidade', $sbaid);
            deletaAssociativaSubacao('pi_subacaoenquadramento', $sbaid);
            deletaAssociativaSubacao('pi_acaoestrategicasubacao', $sbaid);
            deletaAssociativaSubacao('pi_subacaodotacao', $sbaid);
            deletaAssociativaSubacao('produto_spo_subacao', $sbaid, 'id_subacao', 'planacomorc');
        }

        // -- Insere unidades gestoras
        $arTipos = array('%d', '%s', '%s', '%s');
        $arDados = array(
            'sbaid' => $sbaid,
            'ungcod' => null,
            'unicod' => 'null',
            'unitpocod' => "'U'"
        );

        if($dados['ungcod'])
        foreach ($dados['ungcod'] as $ungcod) {
            $arDados['unicod'] = "'".buscaUniCod($ungcod)."'";
            $arDados['ungcod'] = "'{$ungcod}'";
            insereEmAssociativaSubacao('pi_subacaounidade', $arDados, $arTipos);
        }

        // -- Insere enquadramento da despesa
        $arTipos = array('%d', '%d');
        $arDados = array(
            'sbaid' => $sbaid,
            'eqdid' => null
        );

        if($dados['eqdid'])
        foreach ($dados['eqdid'] as $eqdid) {
            $arDados['eqdid'] = $eqdid;
            insereEmAssociativaSubacao('pi_subacaoenquadramento', $arDados, $arTipos);
        }
       
        // -- Insere a?ões estratégicas -- As modifica?ões comentadas s?o para verifica??o e envio de email
        $AEsModificadas = false;
        $arAcoesEstrategicasAtuais = consultaAcoesEstrategicas($sbaid);

        $arTipos = array('%d', '%d');
        $arDados = array(
            'sbaid' => $sbaid,
            'acaid' => null
        );

        // -- insere ptres
        $arTipos = array('%d', '%d', '%f');
        $arDados = array(
            'sbaid' => $sbaid,
            'ptrid' => null,
            'sadvalor' => null
        );


        // -- Inser??o de novos PTRES na ação
        if (isset($dados['plivalor']) && is_array($dados['plivalor'])) {
            foreach ($dados['plivalor'] as $ptrdata) {
                $arDados['ptrid'] = key($ptrdata);
                $arDados['sadvalor'] = str_replace(array('.', ','), array('', '.'), current($ptrdata));
                insereEmAssociativaSubacao('pi_subacaodotacao', $arDados, $arTipos);
            }
        }
        // -- Inser??o de PTRES que j? estavam assoaciados ao a??o
        if (isset($dados['valor']) && is_array($dados['valor'])) {
            foreach ($dados['valor'] as $ptrid => $ptrvalor) {
                $arDados['ptrid'] = $ptrid;
                $arDados['sadvalor'] = str_replace(array('.', ','), array('', '.'), $ptrvalor);
                insereEmAssociativaSubacao('pi_subacaodotacao', $arDados, $arTipos);
            }
        }
    }

    if ($db->commit()) {
        return true;
    }

    $db->rollback();
    return false;
}

function geraSbacod($pieid, $pigid) {
    global $db;

    $sbacod_p1 = $db->pegaUm(
            sprintf("SELECT piecod FROM monitora.pi_executor WHERE pieid = %d", $pieid)
    );
    $sbacod_p2 = $db->pegaUm(
            sprintf("SELECT pigcod FROM monitora.pi_gestor WHERE pigid = %d", $pigid)
    );
    $sbacod_seq = $db->pegaUm(
            sprintf(
                    "SELECT gspseq
               FROM public.geradorsequencialpi
               WHERE gspidentificador = '%s%s'
               ORDER BY gspid DESC", $sbacod_p1, $sbacod_p2
            )
    );
    if ($sbacod_seq) {
        $sbacod_seq = retornaseq(substr($sbacod_seq, -2));
        $sbacod_seq = str_pad($sbacod_seq, 4, "0", STR_PAD_LEFT);
    } else {
        $sbacod_seq = "0001";
    }

    $db->executar(
            sprintf(
                    "INSERT INTO geradorsequencialpi(gspseq, gspidentificador) VALUES ('%s', '%s%s')", $sbacod_seq, $sbacod_p1, $sbacod_p2
            )
    );

    // No caso da suba??o retorna apenas os dois ?ltimos campos do código gerado.
    $sbacod_p3 = substr($sbacod_seq, -2);

    return "{$sbacod_p1}{$sbacod_p2}{$sbacod_p3}";
}

function deletaAssociativaSubacao($tabela, $sbaid, $campoSubacao = 'sbaid', $esquema = 'monitora') {
    global $db;

    $tabelasPermitidas = array(
        'pi_subacaodotacao',
        'pi_subacaounidade',
        'pi_subacaoenquadramento',
        'pi_acaoestrategicasubacao',
        'produto_spo_subacao'
    );

    // -- Verificando se a tabela informada é uma das permitidas para constru??o
    // -- da query.
    if (!in_array($tabela, $tabelasPermitidas)) {
        return false;
    }

    $sqlAssociacao = "DELETE FROM %s.%s t WHERE t.%s = %d";
    $stmtAssociacao = sprintf($sqlAssociacao, $esquema, $tabela, $campoSubacao, $sbaid);
    return $db->executar($stmtAssociacao);
}

function insereEmAssociativaSubacao($tabela, $arrValores, $arrTipos) {
    global $db;    
    
    if((array_key_exists('eqdid',$arrValores) && trim($arrValores['eqdid']) == '') || (array_key_exists('ungcod',$arrValores) && trim($arrValores['ungcod']) == '\'\''))
        return true;
    
    $sqlAssociacao = "INSERT INTO monitora.%s("
            . implode(', ', array_keys($arrValores)) . ") VALUES(" . implode(', ', $arrTipos) . ")";
    array_unshift($arrValores, $tabela);
    $stmtAssociacao = vsprintf($sqlAssociacao, $arrValores);    
    return $db->executar($stmtAssociacao);
}

function apagarSubacao($sbaid) {
    global $db;
    $sql = <<<DML
UPDATE monitora.pi_subacao
  SET sbastatus = 'I'
  WHERE sbaid = %d
DML;
    $stmt = sprintf($sql, $sbaid);
    $db->executar($stmt);
    return $db->commit();
}

function inserirMetaFisica($arDados) {
    global $db;
    $sql = <<<DML
INSERT INTO planacomorc.produto_spo_subacao(id_subacao, id_produto_spo, meta_fisica)
  VALUES(%d, %d, %d)
DML;

    $stmt = vsprintf($sql, $arDados);
    return $db->executar($stmt);
}

function consultaAcoesEstrategicas($sbaid) {
    global $db;
    $sql = <<<DML
SELECT aca.acaid
  FROM monitora.pi_acaoestrategicasubacao pas
    INNER JOIN painel.acao aca USING(acaid)
  WHERE aca.acastatus = 'A'
    AND pas.sbaid = %d
DML;
    $stmt = sprintf($sql, $sbaid);
    return (array) $db->carregar($stmt);
}

/* Gerar o DOCID para a Suba??o */

function criaDocumentoSubacao($idAcompanhamentoSubacao) {
    global $db;
    if (empty($idAcompanhamentoSubacao))
        return false;
    $docid = pegaDocid($idAcompanhamentoSubacao);
    if (!$docid) {
        $tpdid = WF_TPDID_PLANACOMORC_SUBACAO;
        $docdsc = "Cadastramento de Acompanhamento de Suba??o";
        /*
         * cria documento WORKFLOW
         */
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        if ($idAcompanhamentoSubacao) {
            $sql = "UPDATE planacomorc.acompanhamento_subacao SET
					 docid = " . $docid . "
					WHERE
					id_acompanhamento_subacao = " . $idAcompanhamentoSubacao;
            $db->executar($sql);
            $db->commit();

            /* Atualiza todos os demais Acompanhamentos para o Per?odo na Suba??o */
            $sql = "SELECT id_subacao, id_periodo_referencia FROM planacomorc.acompanhamento_subacao WHERE id_acompanhamento_subacao = {$idAcompanhamentoSubacao} LIMIT 1";

$dados = $db->pegaLinha($sql);
            $idPeriodoReferencia = $dados['id_periodo_referencia'];
            $idSubacao = $dados['id_subacao'];
            if($idPeriodoReferencia){
            $sql = "UPDATE planacomorc.acompanhamento_subacao SET
					 docid = " . $docid . "
					WHERE
                                        id_periodo_referencia = {$idPeriodoReferencia}
					AND id_subacao = {$idSubacao}";
            $db->executar($sql);
            }
            $db->commit();

            return $docid;
        } else {
            return false;
        }
    } else {
        return $docid;
    }
}

/* Verifica o DOCID para a Suba??o */

function pegaDocid($idAcompanhamentoSubacao) {
    global $db;
    $idAcompanhamentoSubacao = (integer) $idAcompanhamentoSubacao;
    $sql = "SELECT
			 docid
			FROM
			 planacomorc.acompanhamento_subacao
			WHERE
			 id_acompanhamento_subacao  = " . $idAcompanhamentoSubacao;
    return (integer) $db->pegaUm($sql);
}

function pegaEstadoAtual($docid) {

    global $db;

    if ($docid) {
        $docid = (integer) $docid;

        $sql = "
			select
				ed.esdid
			from
				workflow.documento d
			inner join
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
        $estado = $db->pegaUm($sql);

        return $estado;
    } else {
        return false;
    }
}

function montaSubTable($idptres) {
    $sql = "
        SELECT
            id_produto_spo_subacao,
            nome,
            descricao,
            valor_fisico,
            '<input onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);calcExecucaoTotal(this);\" title=\"Informa o físico executada do produto '|| nome ||' \" maxlength=\"8\" name=\"produto_spo['|| id_produto_spo_subacao ||'][{$idptres}]\" class=\"form-control produto_spo_'|| psp.id_produto_spo ||'\" value='|| valor_fisico || '></input>' as valor
	FROM planacomorc.produto_spo psp
	INNER JOIN planacomorc.produto_spo_subacao pss USING(id_produto_spo)
	LEFT JOIN planacomorc.acompanhamento_subacao asb ON(asb.id_subacao = pss.id_subacao
            AND asb.id_ptres = {$idptres}
            AND asb.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']})
	LEFT JOIN planacomorc.acompanhamento_subacao_produto_spo pspo USING(id_produto_spo, id_acompanhamento_subacao, id_periodo_referencia)
        WHERE pss.id_subacao = {$_SESSION['planacomorc']['id_subacao']}
            AND psp.st_ativo = 'A'
    ";
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setQuery($sql);
    $listagem->esconderColuna(array('id_produto_spo_subacao', 'descricao', 'valor_fisico'));
    return $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

/**
 * Função utilizada no arquivo monitorarsubacao.inc como callbackdecampo pela Simec_Listagem
 *
 * @global type $db
 * @param type $idptres
 * @return string
 */
function textoComentarioPtres($idptres) {
    $sql = "
        SELECT
            comentario_ptres AS texto_comentario_ptres
	FROM planacomorc.acompanhamento_subacao
	WHERE id_ptres = {$idptres} and id_subacao = {$_SESSION['planacomorc']['id_subacao']} and id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
    ";
    global $db;
    $comentario = $db->pegaUm($sql);
    $texto = "<textarea class=\"form-control verificacao\" cols=\"8\" rows=\"5\"
		name=\"comentario_ptres[$idptres]\" id=\"comentario_ptres[$idptres]\">";
    $texto .= $comentario;
    $texto .= "</textarea>";
    return $texto;
}

/**
 * Função utilizada no arquivo monitorarsubacao.inc como callbackdecampo pela Simec_Listagem
 * @global type $db
 * @param type $idptres
 * @return type
 */
function fisicoExecutado($idptres) {
    $sql = "
        SELECT
            fisico_convertido_acao AS fisico_convertido_acao
	FROM planacomorc.acompanhamento_subacao
	WHERE id_subacao = '{$_SESSION['planacomorc']['id_subacao']}'
            AND id_ptres = '{$idptres}'
            AND id_periodo_referencia = '{$_REQUEST['id_periodo_referencia']}'
    ";    
    global $db;
    $fisico = (int) $db->pegaUm($sql);
    $texto = "<input value=\"$fisico\" type=\"text\" name=\"fisico_convertido_acao[$idptres]\"
		onkeyup=\"this.value=mascaraglobal('#.###.###.###',this.value);\"
        onblur=\"this.value=mascaraglobal('#.###.###.###',this.value);\"
		onfocus=\"MouseClick(this);this.select();\" class=\"form-control verificacao number\">";
    return $texto;
}

function buscaUniCod($ungcod){
    if(!ungcod)
        return null;

    global $db;

    $sql = <<<DML
        SELECT
            unicod
        FROM public.unidadegestora
        WHERE ungcod = '{$ungcod}'
DML;

    return $db->pegaUm($sql);
}