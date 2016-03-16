<?php
/**
 * Listagem de ações para acompanhamento.
 *
 * A exibição dos dados dessa listagem é alterada pelas permissões do perfil de coordenador de ações.
 *
 * $Id: listarAcao.inc 100482 2015-07-27 14:22:27Z maykelbraz $
 * @filesource
 */

// -- Período de referência atual
$prfid = isset($_REQUEST['periodo'])?$_REQUEST['periodo']:$prfid = retornaPeriodoAtual('A');

/**
 * Filtra apenas as unidades com ações vinculadas ao coordenador de ações
 */
$perfil = pegaPerfilGeral();
$permissoes = Acomporc_Model_AcompanhamentoLocalizador::getPermissaoAcoes($_SESSION['usucpf'], $perfil);

$query = <<<DML
SELECT DISTINCT ssl.unicod,
                ssl.unicod || ' - ' || uni.unidsc AS unidade,
                ssl.prfid,
                COALESCE(usu.usunome, 'UNIDADE SEM MONITOR INTERNO') AS usunome,
                usu.usucpf,
                usu.usufoneddd,
                usu.usufonenum,
                rpu.rpuid
  FROM acomporc.snapshotlocalizador ssl
    INNER JOIN public.unidade uni USING(unicod)
    LEFT JOIN acomporc.usuarioresponsabilidade rpu
      ON (rpu.unicod = ssl.unicod
          AND rpu.rpustatus = 'A'
          AND rpu.pflcod = %d)
    LEFT JOIN seguranca.usuario usu USING(usucpf)
  WHERE ssl.prfid = {$prfid}
    {$permissoes}
  ORDER BY unidade
DML;

$query = sprintf($query, PFL_MONITOR_INTERNO);
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$listagem->setQuery($query);
$listagem->setCabecalho(array('Unidade Orçamentaria', 'Monitor Interno'));

if (!in_array(PFL_MONITOR_INTERNO, $perfil)) {
    $listagem->addAcao('user', array('func' => 'alterarResponsavel', 'extra-params' => array('prfid', 'rpuid', 'usucpf')));
}
$listagem->addAcao('plus', array('func' => 'detalharLocalizadores', 'extra-params' => array('prfid')));
$listagem->addCallbackDeCampo('unidade', 'alinhaParaEsquerda');
$listagem->addCallbackDeCampo('usunome', 'apresentaUsuarioTelefone');
$listagem->esconderColuna(array('prfid', 'rpuid', 'usucpf', 'usufoneddd', 'usufonenum'));
$listagem->turnOnPesquisator();
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

