<?php

class Supervisao_Os extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_os";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sosid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sosid' => null,
        'sgeid' => null,
        'usucpf' => null,
        'docid' => null,
        'semid' => null,
        'sosnum' => null,
        'sosdtemissao' => null,
        'sosdtcadastro' => null,
        'sosdtinicio' => null,
        'sosdttermino' => null,
        'sosemergencial' => null,
        'sosdiasexecucao' => null,
        'sosvalortotal' => null,
        'sosvalorpago' => null,
        'sosdtautorizacao' => null,
        'sosdtaceitecontratada' => null,
        'sosstatus' => null,
        'sosobs' => null,
        'sostipo' => null,
        'sosterreno' => null
    );

    public function possuiNovaOSPorEmpid($empid, Array $param = array()) {
        $where = array();

        if ($param['esdid']) {
            $param['esdid'] = (array) $param['esdid'];
            $where[] = "ed.esdid IN(" . implode(", ", $param['esdid']) . ")";
        }

        $sql = "SELECT
					COUNT(os.sosid)
				FROM
					obras2.supervisao_os os
				JOIN obras2.supervisao_os_obra oo ON oo.sosid = os.sosid AND
													 oo.empid = {$empid} AND
													 oo.soostatus = 'A'
				JOIN workflow.documento		   d ON d.docid = os.docid					
				JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					os.sosstatus = 'A' AND
					os.sosid NOT IN (SELECT
										sosid
									  FROM
									  	obras2.supervisaoempresa se
									  WHERE
									  	se.suestatus = 'A' AND
										se.empid = {$empid})
					" . ( count($where) ? " AND " . implode(" AND ", $where) : "" );
        $dado = $this->pegaUm($sql);

        return ($dado ? $dado : false);
    }

    public function listaComboFiltroSupervisaoEmpresa($empid, $sosid = null) {
        $sql = "SELECT
					os.sosid AS codigo,
					os.sosnum AS descricao
				FROM
					obras2.supervisao_os os
				JOIN obras2.supervisao_os_obra oo ON oo.sosid = os.sosid AND
													 oo.empid = {$empid} AND
													 oo.soostatus = 'A'
				JOIN workflow.documento d ON d.docid = os.docid AND
											 d.esdid = " . (ESDID_OS_EXECUCAO) . "
				WHERE
					os.sosstatus = 'A' AND
					os.sosid NOT IN (SELECT
										sosid
									  FROM
									  	obras2.supervisaoempresa se
									  WHERE
									  	se.suestatus = 'A' AND
									  	" . ($sosid ? " se.sosid != {$sosid} AND " : "") . "
										se.empid = {$empid})";
        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }

    public function listaDados(Array $param = array()) {
//		$join = array();
        $where = array();

        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {

            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            $arEntidEmpresa = ($arEntidEmpresa ? $arEntidEmpresa : array(0));

            if ($arEntidEmpresa) {
                $arEntidEmpresa = (array) $arEntidEmpresa;
                $where[] = "ge.entid IN('" . implode("', '", $arEntidEmpresa) . "')";
            }
        }

        $sql = "SELECT
					sosid,
					sosnum,
					TO_CHAR(sosdtinicio, 'dd/mm/yyyy') AS sosdtinicio,
					TO_CHAR(sosdttermino, 'dd/mm/yyyy') AS sosdttermino,
					sosvalortotal,
					sgrnum::varchar || ' - ' || sgrnome AS sgrnome,
					e.entnome,
					em.semnumempenho,
					em.semvalor
				FROM
					obras2.supervisao_os os
				JOIN obras2.supervisao_grupo_empresa ge ON ge.sgeid = os.sgeid AND
									   					   ge.sgestatus = 'A'
				JOIN entidade.entidade e ON e.entid = ge.entid
				JOIN obras2.supervisao_grupo gr ON gr.sgrid = ge.sgrid AND
								   				   gr.sgrstatus = 'A'
				JOIN obras2.supervisao_empenho em ON em.sgeid = ge.sgeid AND
								     				 em.semstatus = 'A'
				WHERE
					sosstatus = 'A'
					" . ($where ? " AND " . implode(" AND ", $where) : "") . "
				ORDER BY
					sosdtcadastro DESC";
        $dado = $this->carregar($sql);
        return ($dado ? $dado : array());
    }

    public function listaOS(array $arrWhere, $xls = false) {
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC))) {

            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            $arEntidEmpresa = ($arEntidEmpresa ? $arEntidEmpresa : array(0));

            if ($arEntidEmpresa) {
                $arEntidEmpresa = (array) $arEntidEmpresa;

                if ($arrWhere['entid'])
                    $arEntidEmpresa = (in_array($arrWhere['entid'], $arEntidEmpresa)) ? array($arrWhere['entid']) : $arEntidEmpresa;

                $where[] = "ge.entid IN('" . implode("', '", $arEntidEmpresa) . "')";
            }
        } else {
            if ($arrWhere['entid'])
                $where[] = "ge.entid = {$arrWhere['entid']}";
        }

        if (possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_GESTOR))) {
            $where[] = "doc.esdid != " . ESDID_OS_CADASTRAMENTO;
        }
        if ($arrWhere['sgrid']) {
            $where[] = "gr.sgrid = {$arrWhere['sgrid']} ";
        }
        if ($arrWhere['ge.sgeid']) {
            $where[] = "sgeid = {$arrWhere['sgeid']} ";
        }
        if ($arrWhere['em.semid']) {
            $where[] = "semid = {$arrWhere['semid']} ";
        }
        if ($arrWhere['sosnum']) {
            $where[] = "sosnum::text ilike ('%{$arrWhere['sosnum']}%') ";
        }
        if ($arrWhere['sosemergencial'] == "t") {
            $where[] = "sosemergencial is true ";
        }
        if ($arrWhere['sosemergencial'] == "f") {
            $where[] = "sosemergencial is false ";
        }
        if ($arrWhere['esdid']) {
            $where[] = "esd.esdid = {$arrWhere['esdid']}";
        }

        // Verifica se alguma supervisao foi homologada com atraso com base na data de termino da OS
        if ($arrWhere['h_atrasada'] == "sim") {
            $where[] = "os.sosid IN (SELECT sos.sosid
                            FROM obras2.supervisao_os sos
                            JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid AND sue.suestatus = 'A'
                            JOIN obras2.supervisao s ON s.sueid = sue.sueid AND s.supstatus = 'A'
                            JOIN workflow.documento d ON sue.docid = d.docid
                            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                            JOIN obras2.obras o ON o.obrid = s.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
                            WHERE

                                (e.esdid IN (734, 756, 757) AND (sos.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d.docid GROUP BY docid)::date))
                                AND sos.sosdttermino < NOW()
                                GROUP BY sos.sosid)";
        }
        if ($arrWhere['h_atrasada'] == "nao") {
            $where[] = "os.sosid NOT IN (SELECT sos.sosid
                            FROM obras2.supervisao_os sos
                            JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid AND sue.suestatus = 'A'
                            JOIN obras2.supervisao s ON s.sueid = sue.sueid AND s.supstatus = 'A'
                            JOIN workflow.documento d ON sue.docid = d.docid
                            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                            JOIN obras2.obras o ON o.obrid = s.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
                            WHERE

                                (e.esdid IN (734, 756, 757) AND (sos.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d.docid GROUP BY docid)::date))
                                AND sos.sosdttermino < NOW()
                                GROUP BY sos.sosid)";
        }

        //obras2.supervisaoempresa pegar o campo suedtsupervisao e ver se a data de hj é maior
        if ($arrWhere['rdb_atrasada'] == "sim") {
            $where[] = "sue.suedtsupervisao > now()";
        }
        if ($arrWhere['rdb_atrasada'] == "nao") {
            $where[] = "sue.suedtsupervisao <= now()";
        }


        if ($arrWhere['sosterreno'] == "t") {
            $where[] = "os.sosterreno = 't'";
        }
        if ($arrWhere['sosprorrogada'] == "f") {
            $where[] = "os.sosterreno = 'f'";
        }


        if ($arrWhere['sosprorrogada'] == "t") {
            $where[] = "prp.prposid IS NOT NULL";
        }
        if ($arrWhere['sosprorrogada'] == "f") {
            $where[] = "prp.prposid IS NULL";
        }
        if ($arrWhere['listobraestuf']) {
            $arrWhere['listobraestuf'] = (array) $arrWhere['listobraestuf'];
            $where[] = "tes.estuf IN('" . implode("', '", $arrWhere['listobraestuf']) . "')";
        }

        if ($arrWhere['listobraestuf']) {
            $arrWhere['listobraestuf'] = (array) $arrWhere['listobraestuf'];
            $where[] = "tes.estuf IN('" . implode("', '", $arrWhere['listobraestuf']) . "')";
        }


        if (!empty($arrWhere['inicio_exec_de'])) {
            $where[] =  " os.sosdtinicio::date >= '" . ajusta_data($arrWhere['inicio_exec_de']) . "' ";
        }
        if (!empty($arrWhere['inicio_exec_ate'])) {
            $where[] = " os.sosdtinicio::date <= '" . ajusta_data($arrWhere['inicio_exec_ate']) . "' ";
        }
        
        if (!empty($arrWhere['termino_exec_de'])) {
            $where[] =  " os.sosdttermino::date >= '" . ajusta_data($arrWhere['termino_exec_de']) . "' ";
        }
        if (!empty($arrWhere['termino_exec_ate'])) {
            $where[] = " os.sosdttermino::date <= '" . ajusta_data($arrWhere['termino_exec_ate']) . "' ";
        }

        if (!empty($arrWhere['dt_emissao_de'])) {
            $where[] =  " os.sosdtemissao::date >= '" . ajusta_data($arrWhere['dt_emissao_de']) . "' ";
        }
        if (!empty($arrWhere['dt_emissao_ate'])) {
            $where[] = " os.sosdtemissao::date <= '" . ajusta_data($arrWhere['dt_emissao_ate']) . "' ";
        }





      //  ver($where,d);

        if ($arrWhere['listobramescod']) {
            $where[] = " mes.mescod = '" . $arrWhere['listobramescod'] . "'";
        }
        if ($arrWhere['listobramiccod']) {
            $where[] = " mic.miccod = '" . $arrWhere['listobramiccod'] . "'";
        }
        if ($arrWhere['listobramuncod']) {
            $where[] = " m.muncod = '" . $arrWhere['listobramuncod'] . "'";
        }


        $imgExc = '';
        if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
            $imgExc = "<img class=\'link\' src=\'../imagens/excluir.gif\' onclick=\'excluirOs(' || os.sosid || ')\'  >";
        }

        $btnProrrogacao = "";
        if (possui_perfil(array(PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC)) || possui_perfil(array(PFLCOD_SUPER_USUARIO)))
            $btnProrrogacao = "<img title=\'Prorrogar Prazo\' src=\'../imagens/calendario.gif\' class=\'link\' onclick=\'abrePopupProrrogacao(' || os.sosid || ')\' />";

        if ($xls) {
            $acao = '';
        } else {
            $acao = "'<img class=\'link\' src=\'../imagens/alterar.gif\' onclick=\'editarOs(' || os.sosid || ')\'  >
					{$imgExc}
					{$btnProrrogacao}
					 <img class=\'link\' src=\'../imagens/print.png\' onclick=\'imprimirOs(' || os.sosid || ')\'  >' as acao,";
        }


        $sql = "SELECT
					$acao
					'' || sosnum,
                                        array_to_string(
                                            array(
                                                select distinct on (ed.estuf) ed.estuf 
                                                from obras2.supervisao_os_obra soo1
                                                join obras2.empreendimento emp on soo1.empid = emp.empid AND
                                                          emp.empstatus = 'A'
                                                join entidade.endereco ed on ed.endid = emp.endid AND tpeid = " . TIPO_ENDERECO_OBJETO . "
                                                where soo1.sosid = os.sosid and soo1.soostatus = 'A'
                                                order by ed.estuf
                                        ), ', ') as uf,
					TO_CHAR(sosdtinicio, 'dd/mm/yyyy') AS sosdtinicio,
					TO_CHAR(sosdttermino, 'dd/mm/yyyy') AS sosdttermino,
					sosvalortotal,
					e.entnome,
					em.semnumempenho,
					(select count(soo.empid) from obras2.supervisao_os_obra soo where soo.sosid = os.sosid and soostatus = 'A') as qtde_obras,
					coalesce(esd.esddsc,'Não Iniciada') as esddsc,
					(select COUNT(*) as c from obras2.supervisao_os_obra soo
					  JOIN obras2.supervisao_os sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'
					  JOIN obras2.supervisaoempresa se ON se.sosid = soo.sosid  AND se.empid = soo.empid AND se.suestatus = 'A'
					  where soo.sosid = os.sosid and soostatus = 'A') as qtde_supervisao,
					(select COUNT(*) as c from obras2.supervisao_os_obra soo
					  JOIN obras2.supervisao_os sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'
                      JOIN obras2.supervisaoempresa se ON se.sosid = soo.sosid  AND se.empid = soo.empid AND se.suestatus = 'A'
                      JOIN workflow.documento doc2 ON doc2.docid = se.docid
                      JOIN workflow.estadodocumento esd2 ON doc2.esdid = esd2.esdid and esd2.esdid = " . WF_ESDID_LAUDO_SUPERVISAO_PAGO . "
                      where soo.sosid = os.sosid and soostatus = 'A') as qtde_supervisao_pagas,
                    (select COUNT(*) as c from obras2.supervisao_os_obra soo
					  JOIN obras2.supervisao_os sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'
					      JOIN obras2.supervisaoempresa se ON se.sosid = soo.sosid  AND se.empid = soo.empid AND se.suestatus = 'A'
					      JOIN workflow.documento doc2 ON doc2.docid = se.docid
					      JOIN workflow.estadodocumento esd2 ON doc2.esdid = esd2.esdid and esd2.esdid = 1188
					      where soo.sosid = os.sosid and soostatus = 'A') as qtde_supervisao_canceladas,
                    (CASE
						WHEN os.sosemergencial=true THEN 'SIM'
						WHEN os.sosemergencial=false THEN 'NÃO'
					END) as sosemergencial
				FROM
					obras2.supervisao_os os
				LEFT JOIN obras2.prorrogacao_prazo_os prp ON prp.sosid = os.sosid
				LEFT JOIN workflow.documento doc ON doc.docid = os.docid
				LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
				JOIN obras2.supervisao_grupo_empresa ge ON ge.sgeid = os.sgeid AND ge.sgestatus != 'I'
				JOIN entidade.entidade e ON e.entid = ge.entid
				LEFT JOIN entidade.endereco edr ON edr.entid = e.entid
				JOIN territorios.estado tes ON tes.estuf = edr.estuf
				LEFT JOIN territorios.municipio m ON m.muncod = edr.muncod
				LEFT JOIN territorios.mesoregiao mes ON mes.mescod = m.mescod
				LEFT JOIN territorios.microregiao mic ON mic.miccod = m.miccod
				JOIN obras2.supervisao_grupo gr ON gr.sgrid = ge.sgrid AND
								   				   gr.sgrstatus = 'A'
				JOIN obras2.supervisao_empenho em ON os.semid = em.semid AND em.semstatus = 'A'
				WHERE
					sosstatus = 'A'
					" . ($where ? " AND " . implode(" AND ", $where) : "") . "
				GROUP BY
					tes.estdescricao,os.sosid,os.sosnum,os.sosdtinicio,os.sosdttermino,os.sosvalortotal,e.entnome,em.semnumempenho,esd.esddsc,os.sosdtcadastro,os.sosemergencial
				ORDER BY
					sosdtcadastro DESC";

        $arrCabecalho = array("Ação", "OS Nº", "UF", "Início da Execução", "Término da Execução", "Valor Total dos Serviços", "Empresa", "Nº Empenho", "Qtde. Obras", "Situação da OS", "Qtde. Supervisões", "Qtde. Supervisões Pagas", "Qtde. Supervisões Canceladas", "Emergencial");

        $paramLista['managerOrder'] = array(
            3 => 'sosdtinicio',
            4 => 'sosdttermino',
        );

        if ($xls) {
            $arrCabecalho = array("OS Nº", "UF", "Início da Execução", "Término da Execução", "Valor Total dos Serviços", "Empresa", "Nº Empenho", "Qtde. Obras", "Situação da OS", "Qtde. Supervisões", "Qtde. Supervisões Pagas", "Qtde. Supervisões Canceladas", "Emergencial");
            header('content-type: text/html; charset=ISO-8859-1');
            $this->sql_to_xml_excel($sql, 'listaOS', $arrCabecalho, '');
        } else {
            $this->monta_lista($sql, $arrCabecalho, 100, 10, "S", "center", "N", null, null, null, null, $paramLista);
        }
    }

    /**
     * DEPRECATED
     * O metodo listaOS já possuí a funcionalidade de ferar execel
     * @param array $arrWhere
     */
    public function listaOSExportar(array $arrWhere) {
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            $arEntidEmpresa = ($arEntidEmpresa ? $arEntidEmpresa : array(0));

            if ($arEntidEmpresa) {
                $arEntidEmpresa = (array) $arEntidEmpresa;
                $where[] = "ge.entid IN('" . implode("', '", $arEntidEmpresa) . "')";
            }
        }
        if ($arrWhere['sgrid']) {
            $where[] = "gr.sgrid = {$arrWhere['sgrid']} ";
        }
        if ($arrWhere['ge.sgeid']) {
            $where[] = "sgeid = {$arrWhere['sgeid']} ";
        }
        if ($arrWhere['em.semid']) {
            $where[] = "semid = {$arrWhere['semid']} ";
        }
        if ($arrWhere['sosnum']) {
            $where[] = "sosnum::text ilike ('%{$arrWhere['sosnum']}%') ";
        }
        if ($arrWhere['sosemergencial'] == "t") {
            $where[] = "sosemergencial is true ";
        }
        if ($arrWhere['sosemergencial'] == "f") {
            $where[] = "sosemergencial is false ";
        }
        if ($arrWhere['esdid']) {
            $where[] = "esd.esdid = {$arrWhere['esdid']}";
        }
        //obras2.supervisaoempresa pegar o campo suedtsupervisao e ver se a data de hj é maior
        if ($arrWhere['rdb_atrasada'] == "sim") {
            $where[] = "sue.suedtsupervisao > now()";
        }
        if ($arrWhere['rdb_atrasada'] == "nao") {
            $where[] = "sue.suedtsupervisao <= now()";
        }

        $sql = "SELECT
					sosnum,
					TO_CHAR(sosdtinicio, 'dd/mm/yyyy') AS sosdtinicio,
					TO_CHAR(sosdttermino, 'dd/mm/yyyy') AS sosdttermino,
					sosvalortotal,
					e.entnome,
					em.semnumempenho,
					(select count(soo.empid) from obras2.supervisao_os_obra soo where soo.sosid = os.sosid and soostatus = 'A') as qtde_obras,
					esd.esddsc,
					count(sue.sueid) as qtde_supervisao,
					count(esd2.esdid) as qtde_supervisao_pagas
				FROM
					obras2.supervisao_os os
				JOIN workflow.documento doc ON doc.docid = os.docid
				JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
				JOIN obras2.supervisao_grupo_empresa ge ON ge.sgeid = os.sgeid AND
									   					   ge.sgestatus = 'A'
				JOIN entidade.entidade e ON e.entid = ge.entid
				JOIN obras2.supervisao_grupo gr ON gr.sgrid = ge.sgrid AND
								   				   gr.sgrstatus = 'A'
				JOIN obras2.supervisao_empenho em ON em.sgeid = ge.sgeid AND
								     				 em.semstatus = 'A'
				LEFT JOIN obras2.supervisaoempresa sue ON sue.sosid = os.sosid
				LEFT JOIN workflow.documento doc2 ON doc2.docid = sue.docid
				LEFT JOIN workflow.estadodocumento esd2 ON doc2.esdid = esd2.esdid and esd2.esdid = " . WF_ESDID_LAUDO_SUPERVISAO_PAGO . "
				WHERE
					sosstatus = 'A'
					" . ($where ? " AND " . implode(" AND ", $where) : "") . "
				GROUP BY
					os.sosid,os.sosnum,os.sosdtinicio,os.sosdttermino,os.sosvalortotal,e.entnome,em.semnumempenho,esd.esddsc,os.sosdtcadastro
				ORDER BY
					sosdtcadastro DESC";
        $arrCabecalho = array("OS Nº", "Início da Execução", "Término da Execução", "Valor Total dos Serviços", "Empresa", "Nº Empenho", "Qtde. Obras", "Situação da OS", "Qtde. Supervisões", "Qtde. Supervisões Pagas");
        ob_clean();
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Cache-Control: no-cache, must-revalidate");
        header("Pragma: no-cache");
        header("Content-type: application/x-msexcel");
        header("Content-Disposition: attachment; filename=\"lista_os.xls\"");
        $this->monta_lista_tabulado($sql, $arrCabecalho, 100, 10, "S", "center", "N");
        die;
    }

    function pegaUltimaAtualizadaoSupervisao($sueid, $homol = false) {
        global $db;
        $aeddestino = ($homol) ? 734 : 733;

        $docid = pegaDocidSupervisaoEmpresa($sueid);
        $sql = "select
                                                                            ed.esddsc,
                                                                            ac.aeddscrealizada,
                                                                            us.usunome,
                                                                            us.usucpf,
                                                                            TO_CHAR(hd.htddata, 'DD/MM/YYY') as htddata,
                                                                            cd.cmddsc
                                                                        from workflow.historicodocumento hd
                                                                            inner join workflow.acaoestadodoc ac on
                                                                                ac.aedid = hd.aedid
                                                                            inner join workflow.estadodocumento ed on
                                                                                ed.esdid = ac.esdidorigem
                                                                            inner join seguranca.usuario us on
                                                                                us.usucpf = hd.usucpf
                                                                            left join workflow.comentariodocumento cd on
                                                                                cd.hstid = hd.hstid
                                                                        where
                                                                            hd.docid = " . $docid . " AND ac.esdiddestino = {$aeddestino}
                                                                        order by
                                                                            hd.hstid DESC LIMIT 1";

        return $db->pegaLinha($sql);
    }

    /**
     * Returna todas as obras de determinada OS
     * @param null $sosid
     * @param bool $cancelada
     * @return array|void
     */
    function pegaObrasOs($sosid = null, $cancelada = true) {
        $sosid = (is_null($sosid)) ? $this->sosid : $sosid;
        $sql = "SELECT soo.* FROM obras2.supervisao_os os
                JOIN obras2.supervisao_os_obra soo ON soo.sosid = os.sosid AND soo.soostatus = 'A'
                WHERE os.sosid = {$sosid}";

        if ($cancelada)
            $sql = "SELECT soo.* FROM obras2.supervisao_os os
                JOIN obras2.supervisao_os_obra soo ON soo.sosid = os.sosid AND soo.soostatus = 'A'
                JOIN obras2.supervisaoempresa sue ON sue.empid = soo.empid AND sue.sosid = os.sosid AND sue.suestatus = 'A'
                JOIN workflow.documento d ON d.docid = sue.docid
                WHERE os.sosid = {$sosid} AND d.esdid != 1188";

        return $this->carregar($sql);
    }

    /**
     * Função responsável por recaulcular o valor da OS com base nas obras ativas e não canceladas numa ação futura
     * Query de auxilio
     * SELECT os.sosid, os.sosnum, os.sosvalortotal, count(soo.empid), sge.sgevalorunitario, sge.sgevalorb,
     *   CASE WHEN count(soo.empid) > 10 THEN
     *   count(soo.empid)*sge.sgevalorb
     *   ELSE
     *   count(soo.empid)*sge.sgevalorunitario
     *   END as vtotalfinal
     *   FROM obras2.supervisao_os os
     *   JOIN obras2.supervisao_os_obra soo ON soo.sosid = os.sosid AND soo.soostatus = 'A'
     *   JOIN obras2.supervisaoempresa sue ON sue.empid = soo.empid AND sue.sosid = os.sosid AND sue.suestatus = 'A'
     *   JOIN workflow.documento d ON d.docid = sue.docid
     *   JOIN obras2.supervisao_grupo_empresa sge ON sge.sgeid = os.sgeid
     *   WHERE d.esdid != 1188 -- canceladas
     *   GROUP BY os.sosid, os.sosvalortotal, sge.sgevalorunitario, sge.sgevalorb, os.sosnum
     *   HAVING
     *   (CASE WHEN count(soo.empid) > 10 THEN
     *   count(soo.empid)*sge.sgevalorb
     *   ELSE
     *   count(soo.empid)*sge.sgevalorunitario END) != os.sosvalortotal
     *
     * @param $sosid
     */
    function recalculaValorOs($sosid = null) {
        if ($sosid)
            $this->carregarPorId($sosid);

        if ($this->sgeid) {
            $grupoEmpresa = new Supervisao_Grupo_Empresa();
            $valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid($this->sgeid);
            $valorUnitariob = $grupoEmpresa->pegaValorUnitariobPorSgeid($this->sgeid);

            $obras = count($this->pegaObrasOs());
            $valorTot = ($obras > 10) ? $obras * $valorUnitariob : $obras * $valorUnitario;

            $this->sosvalortotal = $valorTot;
            $this->salvar();
            $this->commit();
        }
    }

}
