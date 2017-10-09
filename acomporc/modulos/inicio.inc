<?php
/**
 * Sistema de aompanhamento orçamentário.
 * $Id: inicio.inc 92623 2015-01-13 17:15:40Z fellipesantos $
 */

/**
 * Cabecalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

$sql = " SELECT DISTINCT
		arq.arqid,
		arq.arqnome,
		arq.arqextensao,
		con.angdsc
	 FROM
		spo.anexogeral con
		INNER JOIN public.arquivo arq on con.arqid = arq.arqid
		INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
	 WHERE arqstatus = 'A'
		   AND con.angtipoanexo = 'SU'
	 ORDER BY angdsc
   ";
$listadocumento = $db->carregar($sql);

$sql = " SELECT DISTINCT
            arq.arqid,
            arq.arqnome,
            arq.arqextensao,
            con.angdsc
         FROM
            planacomorc.anexogeral con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
         WHERE arqstatus = 'Af' AND con.angtipoanexo = 'C'
         ORDER BY
            angdsc
       ";

$listaComunicado = $db->carregar($sql);

/**
 * Recupera um array com os menus e seus itens de menus de maneira recursivamente.
 *
 * @global object $db - class
 * @param integer $sisId
 * @param string $cpf
 * @return array $menus
 */
function carregarMenusBox($sisId, $menuPaiId, $cpf) {
    global $db;

    // Carregando o menu e itens de menu deste sistema e dos usuarios especificos.
    $lista_cpf = "'" . $cpf . "'";

    $sql = "select distinct mnu.mnucod, mnu.mnuid, mnu.mnuidpai, mnu.mnudsc, mnu.mnustatus, mnu.mnulink,
			   mnu.mnutipo, mnu.mnustile, mnu.mnuhtml, mnu.mnusnsubmenu, mnu.mnutransacao, mnu.mnushow, mnu.abacod
			   from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
			   where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod
			   and pfu.usucpf in ({$lista_cpf})
			   and ( mnu.mnutipo=1 or mnu.mnuidpai is not null )
			   and mnu.mnushow='t' and mnu.mnustatus='A'
			   and mnu.sisid={$sisId}
			   and mnu.mnuidpai=(select mnuid from seguranca.menu where mnucod = {$menuPaiId} and sisid={$sisId})
			   order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc";

    $menusBd = @$db->carregar($sql);

    return $menusBd;
}
?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">$(document).ready(function () {
        inicio();
    });</script>

<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
             <div class="divGraf bg-gn">
                <span class="tituloCaixa">Acompanhamento de Ações</span>
                <?php
//                $params = array();
//                $params['texto'] = 'Central de Acompanhamento';
//                $params['tipo'] = 'acompanhamento';
//                $params['url'] = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=L&tipo=A';
//                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Cadastrar Acompanhamentos';
                $params['tipo'] = 'listar';
                $params['url'] = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Questionário de Acompanhamento';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Associar Questionários';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Enviar Acompanhamento para o SIOP em LOTE';
                $params['tipo'] = 'enviar';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Períodos de Acompanhamento de Ação';
                $params['tipo'] = 'calendario';
                $params['url'] = 'acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                    $params = array();
                    $params['texto'] = 'Responsáveis';
                    $params['tipo'] = 'user';
                    $params['url'] = '';
                    montaBotaoInicio($params);
                ?>
            </div>
           <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                    $params = array();
                    $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                    $params['tipo'] = 'snapshot';
                    $params['url'] = 'acomporc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                    montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                    $params = array();
                    $params['texto'] = 'Espelho das Ações';
                    $params['tipo'] = 'espelho';
                    $params['url'] = 'acomporc.php?modulo=principal/dadosacoes/espelho&acao=A';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: #00CED1">
                <span class="tituloCaixa">Acompanhamento de Subações</span>
                <?php
                $params = array();
                $params['texto'] = 'Central de Acompanhamento';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = 'acomporc.php?modulo=principal/acompanhamento/central&acao=A&tipo=S';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Preencher Acompanhamento';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Questionário de Acompanhamento';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=S';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Associar Questionários';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=S';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Períodos de Acompanhamento de Subação';
                $params['tipo'] = 'calendario';
                $params['url'] = 'acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=S';
                montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: darksalmon">
                <span class="tituloCaixa">Relatório de Gestão (TCU/CGU)</span>
                <?php
                $params = array();
                $params['texto'] = 'Central de Acompanhamento';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = 'acomporc.php?modulo=principal/acompanhamento/central&acao=A&tipo=T';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Preencher Relatório';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Questionário de Relatório';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=T';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Associar Questionários';
                $params['tipo'] = 'questionario';
                $params['url'] = 'acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=T';
                montaBotaoInicio($params);


                $params = array();
                $params['texto'] = 'Períodos de Acompanhamento';
                $params['tipo'] = 'calendario';
                $params['url'] = 'acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=T';
                montaBotaoInicio($params);
                ?>
            </div>
            <br style="clear:both"/>
            <div id="divManuais" class="divGraf">
                <span class="tituloCaixa" >Manuais</span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Manual completo';
                $params['tipo'] = 'pdf';
                $params['url'] = 'manual.pdf';
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