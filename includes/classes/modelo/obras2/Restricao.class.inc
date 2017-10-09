<?php

class Restricao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.restricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("rstid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array('rstid'                      => null,
                                   'tprid'                      => null,
                                   'fsrid'                      => null,
                                   'empid'                      => null,
                                   'usucpf'                     => null,
                                   'usucpfsuperacao'            => null,
                                   'rstdsc'                     => null,
                                   'rstdtprevisaoregularizacao' => null,
                                   'rstdtsuperacao'             => null,
                                   'rstdscprovidencia'          => null,
                                   'rstsituacao'                => null,
                                   'rstdtinclusao'              => null,
                                   'rststatus'                  => null,
                                   'obrid'                      => null,
                                   'rstitem'                    => null,
                                   'rstid_1'                    => null,
                                   'rsqid'                      => null,
                                   'qtsid'                      => null,
                                   'usucpfedicao'               => null,
                                   'docid'                      => null,
                                   'rstflressalva'              => null,
                                   'rstobsressalva'             => null
                                  );

    public function listaSql(Array $param = array()) {
        $arWhere = array();

        if (!empty($param['empid'])) {
            $param['empid'] = (array) $param['empid'];
            $arWhere[] = "r.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if (!empty($param['obrid'])) {
            $param['obrid'] = (array) $param['obrid'];
            $arWhere[] = "r.obrid IN(" . implode(", ", $param['obrid']) . ")";
        }

        $perfilSuperUser = (possui_perfil(array(PFLCOD_SUPER_USUARIO)) ? 1 : 0);
        $perfilGestorMec = (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC)) ? 1 : 0);
        
        $param['block_imgs_acao'] = (possuiPerfil(array(PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR)) ? true : $param['block_imgs_acao']);

        if ($param['block_imgs_acao'] !== true || $perfilSuperUser) {
            $acao = "'<center><div style=\"width:50px;\">
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarRest(\'' || r.rstid || '\');\"
		 					title=\"Alterar\">' ||
		 				CASE  
		 					WHEN ({$perfilSuperUser} = 1) OR 
		 						 ({$perfilGestorMec} = 1) OR 
		 						 (r.rsqid IS NULL AND r.qtsid IS NULL AND r.usucpfsuperacao IS NULL AND r.usucpf = '{$_SESSION['usucpf']}') THEN 
			 					'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/excluir.gif\"
				 					style=\"cursor: pointer; margin-left: 3px;\"
				 					onclick=\"javascript: excluirRest(\'' || r.rstid || '\');\"
				 					title=\"Excluir\">'
			 				ELSE
			 					'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
			 			END ||
                        CASE
                            WHEN ({$perfilSuperUser} = 1) AND ((SELECT 1 FROM obras2.historico_restricao hr where hr.rstid = r.rstid LIMIT 1) = 1) THEN
                                '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/fluxodocm.gif\"
				 					style=\"cursor: pointer; margin-left: 3px;\"
				 					onclick=\"javascript: abreHistoricoRest(\'' || r.rstid || '\');\"
				 					title=\"Restrições/Inconformidades Relacionadas\">'
                            ELSE
			 					'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
			 			END ||'
                        </center></div>'";
        } else {
            $acao = "''";
        }
        
        $andamento = "CASE 
                            WHEN esd.esdid IN (" . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Correçao\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Correção\">&nbsp;&nbsp;'
                                    END


                            WHEN esd.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . " )
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - rstdtinclusao) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                    END


                            WHEN esd.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . ", " . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_alerta.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'


                            WHEN esd.esdid = " . ESDID_AGUARDANDO_ANALISE_FNDE . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_ativo.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Análise FNDE\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_AGUARDANDO_CORRECAO . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correção\">&nbsp;&nbsp;'
                        END
                         ";

        $analise = "
                        CASE
                            WHEN esd.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . ", " . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_AGUARDANDO_ANALISE_FNDE . "
                                 THEN
                                 CASE
                                        WHEN DATE_PART('days', NOW() - htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                    END

                            WHEN esd.esdid = " . ESDID_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_JUSTIFICADA. "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"justificada\">&nbsp;&nbsp;'

                            WHEN esd.esdid = " . ESDID_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'
                        END
        ";

        $sql = "SELECT
                                {$acao} AS acao,
                                '<center>' || {$andamento} || '</center>' AS andamento, --Andamento
                                r.rstid,
                                CASE WHEN r.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
                                CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
                                tr.tprdsc,
                                TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
                                r.rstdsc,
                                r.rstdscprovidencia,
                                TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,

                                CASE WHEN (SELECT
                                           MAX(p.pflcod)
                                           FROM
                                               seguranca.perfil p 
                                           INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = usu.usucpf AND sisid = 147
                                            WHERE
                                              p.pflstatus='A'
                                            GROUP BY pu.usucpf) = " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . " 
                                THEN
                                    (SELECT DISTINCT
                                          e.entnome AS descricao
                                     FROM
                                          obras2.usuarioresponsabilidade ur
                                     INNER JOIN entidade.entidade e ON e.entid = ur.entid
                                     WHERE
                                        ur.usucpf = usu.usucpf AND
                                        ur.pflcod = " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . " AND
                                        ur.rpustatus='A')
                                ELSE
                                    usu.usunome
                                END AS criadopor,


                                CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
                                esd.esddsc,
                                sup.usunome as ususuperacao,

                                CASE WHEN esd.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ") THEN
                                    'NÃO'
                                WHEN esd.esdid IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ") AND r.rstflressalva = 'S' THEN
                                    'SIM'
                                ELSE 'NÂO' END,

                                usuh.usunome as usuh,
                                TO_CHAR(h.htddata, 'DD/MM/YYYY HH:MM')
                        FROM
                                obras2.restricao r
                        LEFT JOIN obras2.tiporestricao       tr ON tr.tprid = r.tprid AND tr.tprstatus = 'A'
                        LEFT JOIN obras2.faserestricao       fr ON fr.fsrid = r.fsrid AND fr.fsrstatus = 'A'
                        LEFT JOIN seguranca.usuario         usu ON usu.usucpf = r.usucpf
                        LEFT JOIN seguranca.usuario         sup ON sup.usucpf = r.usucpfsuperacao	 
                        LEFT JOIN workflow.documento        doc ON doc.docid  = r.docid
                        LEFT JOIN workflow.estadodocumento esd ON esd.esdid  = doc.esdid
                        LEFT JOIN workflow.historicodocumento h ON doc.hstid = h.hstid
                        LEFT JOIN seguranca.usuario         usuh ON usuh.usucpf = h.usucpf
                        WHERE
                                r.rststatus = 'A' AND " .
                ( implode(" AND ", $arWhere) ) . "
                        ORDER BY
                            r.rstid";
                
        return $sql;
    }
    
    public function salvaRestricaoInconformidade($param, $obrid = null) {
        foreach ($param as $key => $value) {
            if(!is_array($value)) {
              if($value != strip_tags($value)) {
                $param[$key] = strip_tags($value, '<strong><p><br>');
                $param[$key] = addslashes($param[$key]);
              }
            }
        }
        
        if(!empty($param['rstdtprevisaoregularizacao'])){
            $data = explode('/', $param['rstdtprevisaoregularizacao']);
            $param['rstdtprevisaoregularizacao'] = $data[2].'-'.$data[1].'-'.$data[0];
        }
          
        $obra = ($_GET['acao'] != 'A') ? $_SESSION['obras2']['obrid'] : null;
        if($obra == null){
            $obra = $obrid;
        }
        
        $dados = array('rstid'                      => ((empty($param['rstid']) || trim($param['rstid'] == '')) ? null : $param['rstid']),
                       'tprid'                      => ((empty($param['tprid']) || trim($param['tprid'] == '')) ? 12   : $param['tprid']),
                       'fsrid'                      => ((empty($param['fsrid']) || trim($param['fsrid'] == '')) ? null : $param['fsrid']),
                       'empid'                      => ( ( ( $_GET['acao'] == 'A' ) ) ? $_SESSION['obras2']['empid'] : null ),
                       'rstdsc'                     => ((empty($param['rstdsc']) || trim($param['rstdsc'] == '')) ? '' : "'".  addslashes(strip_tags($param['rstdsc'], '<strong><p><br>'))."'"),
                       'rstdtprevisaoregularizacao' => ((empty($param['rstdtprevisaoregularizacao']) || trim($param['rstdtprevisaoregularizacao'] == '')) ? null : "'".$param['rstdtprevisaoregularizacao']."'"),
                       'rstdscprovidencia'          => ((empty($param['rstdscprovidencia']) || trim($param['rstdscprovidencia'] == '')) ? '' : "'".addslashes(strip_tags($param['rstdscprovidencia'], '<strong><p><br>'))."'"),
                       'obrid'                      => $obra,
                       'rstitem'                    => ((empty($param['rstitem']) || trim($param['rstitem'] == '')) ? null : "'".$param['rstitem']."'"),
                       'rstflressalva'              => ((empty($param['rstflressalva']) || trim($param['rstflressalva'] == ''))   ? null : "'".$param['rstflressalva']."'"),
                       'rstobsressalva'             => ((empty($param['rstobsressalva']) || trim($param['rstobsressalva'] == '')) ? '' : "'".addslashes(strip_tags($param['rstobsressalva'], '<strong><p><br>'))."'" )
                      );
            
        if(empty($param['rstid'])){
            
            $dados['rstdtinclusao'] = "now()";
            $dados['rststatus']     = "'A'";
            $dados['usucpf']        = "'".$_SESSION['usucpf']."'";
            
            foreach ($dados as $k => $v) {
                if($v == null){
                    unset($dados[$k]);
                }else{
                    $insert_campos  .= $k.",";
                    $insert_valores .= $v.",";
                }
            }
            $sql_insert = " INSERT INTO obras2.restricao
                                 (".substr($insert_campos,0,-1).") 
                               VALUES 
                               (".substr($insert_valores,0,-1).") 
                               RETURNING rstid; ";
            
            try{
                $rstid = $this->pegaUm($sql_insert);
            } catch (Exception $ex) {
                $this->rollback();
                $arr_return['erro'] = true;
                $arr_return['msg'] .= '\nErro ao cadastrar a Restrição/Inconformidade. '.$ex->getMessage();
            }
            
        }
        else{
            $dados['usucpfedicao']        = "'".$_SESSION['usucpf']."'";
            
            $rstid = $dados['rstid'];
            $update_campos  = "";
            foreach ($dados as $key => $value) {
                if($value == null){
                    unset($dados[$key]);
                }else{
                    $update_campos  .= $key." = ".$value.',';            
                }
            }
            $update = " UPDATE obras2.restricao 
                        SET ".substr($update_campos,0,-1)." 
                        WHERE rstid = ".$rstid;
            
            try{
                $this->executar($update);
            } catch (Exception $ex) {
                $this->rollback();
                $arr_return['erro'] = true;
                $arr_return['msg'] .= '\nErro ao atualizar os dados da Restrição/Inconformidade. Erro: '.$ex->getMessage();
            }
            
        }
        
        $this->carregarPorId($rstid);
        return $rstid;
    }

    public function salvaRestricaoCO($param, $obrid = null) {
        foreach ($param as $key => $value) {
            if(!is_array($value)) {
              if($value != strip_tags($value)) {
                $param[$key] = strip_tags($value, '<strong><p><br>');
                $param[$key] = addslashes($param[$key]);
              }
            }
        }

        if(!empty($param['rstdtprevisaoregularizacao'])){
            $data = explode('/', $param['rstdtprevisaoregularizacao']);
            $param['rstdtprevisaoregularizacao'] = $data[2].'-'.$data[1].'-'.$data[0];
        }

        $obra = ($_GET['acao'] != 'A') ? $_SESSION['obras2']['obrid'] : null;
        if($obra == null){
            $obra = $obrid;
        }

        $dados = array('rstid'                      => ((empty($param['rstid']) || trim($param['rstid'] == '')) ? null : $param['rstid']),
                       'tprid'                      => ((empty($param['tprid']) || trim($param['tprid'] == '')) ? 12   : $param['tprid']),
                       'fsrid'                      => ((empty($param['fsrid']) || trim($param['fsrid'] == '')) ? null : $param['fsrid']),
                       'empid'                      => ( ( ( $_GET['acao'] == 'A' ) ) ? $_SESSION['obras2']['empid'] : null ),
                       'rstdsc'                     => ((empty($param['rstdsc']) || trim($param['rstdsc'] == '')) ? '' : "'".  addslashes(strip_tags($param['rstdsc'], '<strong><p><br>'))."'"),
                       'rstdtprevisaoregularizacao' => ((empty($param['rstdtprevisaoregularizacao']) || trim($param['rstdtprevisaoregularizacao'] == '')) ? null : "'".$param['rstdtprevisaoregularizacao']."'"),
                       'rstdscprovidencia'          => ((empty($param['rstdscprovidencia']) || trim($param['rstdscprovidencia'] == '')) ? '' : "'".addslashes(strip_tags($param['rstdscprovidencia'], '<strong><p><br>'))."'"),
                       'obrid'                      => $obra,
                       'rstitem'                    => ((empty($param['rstitem']) || trim($param['rstitem'] == '')) ? null : "'".$param['rstitem']."'"),
                       'rstflressalva'              => ((empty($param['rstflressalva']) || trim($param['rstflressalva'] == ''))   ? null : "'".$param['rstflressalva']."'"),
                       'rstobsressalva'             => ((empty($param['rstobsressalva']) || trim($param['rstobsressalva'] == '')) ? '' : "'".addslashes(strip_tags($param['rstobsressalva'], '<strong><p><br>'))."'" )
                      );

        $dados['rstdtinclusao'] = "now()";
        $dados['rststatus']     = "'A'";
        $dados['usucpf']        = "''";

        foreach ($dados as $k => $v) {
            if($v == null){
                unset($dados[$k]);
            }else{
                $insert_campos  .= $k.",";
                $insert_valores .= $v.",";
            }
        }
        $sql_insert = " INSERT INTO obras2.restricao
                             (".substr($insert_campos,0,-1).")
                           VALUES
                           (".substr($insert_valores,0,-1).")
                           RETURNING rstid; ";

        try{
            $rstid = $this->pegaUm($sql_insert);
            $this->carregarPorId($rstid);
            $this->atualizaDocidNullRetricao();
        } catch (Exception $ex) {
            $this->rollback();
            $arr_return['erro'] = true;
            $arr_return['msg'] .= '\nErro ao cadastrar a Restrição/Inconformidade. '.$ex->getMessage();
        }
    }

    public function carregaDadosPorObrid($obrid) {
        $sql = "SELECT
					u.usucpf AS usucpf,
					u.usunome AS usunome,
					usup.usucpf AS usucpfsuperacao,
					usup.usunome AS usunomesuperacao,
					CASE 
						WHEN r.fsrid IS NOT NULL THEN 
							fsrdsc 
						ELSE 
							'Não Informada' 
					END AS fsrdsc,
					tr.tprdsc,
					r.rstid,
					r.rstdsc,
					r.rstdscprovidencia,
					TO_CHAR(r.rstdtinclusao,'DD/MM/YYYY') AS rstdtinclusao,
					TO_CHAR(r.rstdtprevisaoregularizacao,'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
					CASE 
						WHEN r.rstsituacao = true THEN 
							to_char(r.rstdtsuperacao,'DD/MM/YYYY') 
						ELSE 'Não' 
					END AS rstdtsuperacao
				FROM
					obras2.restricao r
				JOIN obras2.tiporestricao tr ON tr.tprid = r.tprid
				LEFT JOIN obras2.faserestricao fr ON fr.fsrid = r.fsrid
				JOIN seguranca.usuario u ON u.usucpf = r.usucpf
				LEFT JOIN seguranca.usuario usup ON usup.usucpf = r.usucpfsuperacao
				WHERE
					r.obrid = {$obrid}";
        $dados = $this->carregar($sql);

        return ($dados ? $dados : array());
    }
    
    public function getFiltrosDadosListaRestricao() {
        $where = array();
        
        extract($_REQUEST);
        
        if (!empty($obrbuscatexto)) {
            $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrbuscatexto))));
            $where[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $obrbuscatexto . "%') ) ";
        }

        if (!empty($tpoid)) {

            if(is_array($tpoid) ){
                if($tpoid[0] != ''){
                    $tpoid = implode(', ', $tpoid);
                    $where[] = " obr.tpoid IN ($tpoid) ";
                }
            } else {
                $where[] = " obr.tpoid = $tpoid ";
            }
        }

        if (!empty($empesfera)) {
            $where[] = " ep.empesfera = '$empesfera' ";
        }

        if (!empty($item_restrict)) {
            if ($item_restrict != 'T') {
                $where[] = " rst.rstitem = '$item_restrict' ";
            } else {
                $where[] = " rst.rstitem IN ('R','I')  ";
            }
        }

        if (!empty($estuf)) {
            if (is_array($estuf)) {
                foreach ($estuf as $k => $v) {
                    $estuf[$k] = "'{$v}'";
                }
            } else {
                $estuf = array("'{$estuf}'");
            }
            foreach ($estuf as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($estuf[$k]);
                }
            }
            if (!empty($estuf)) {
                $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
            }
        }
        
        if (!empty($muncod)) {
            if (is_array($muncod)) {
                foreach ($muncod as $k => $v) {
                    $muncod[$k] = "'{$v}'";
                }
            } else {
                $muncod = array("'{$muncod}'");
            }
            foreach ($muncod as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($muncod[$k]);
                }
            }
            if (!empty($muncod)) {
                $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
            }
        }

        if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
            $where[] = " esd_obr.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
        }
        if (!empty($tprid)) {
            $where[] = " tr.tprid = $tprid ";
        }
        
        if (!empty($esdid_ri) && is_array($esdid_ri) && $esdid_ri[0] != '') {
            $where[] = " esd_ri.esdid IN ( " . implode(",", $esdid_ri) . " ) ";
        }
        
        if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
            $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
        }
        
        if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
            $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
        }
        
        if(!empty($rstdtinclusao_de) && !empty($rstdtinclusao_ate)){
            $de  = explode('/', $rstdtinclusao_de);
            $ate = explode('/', $rstdtinclusao_ate);
            $rstdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $rstdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " rst.rstdtinclusao BETWEEN '{$rstdtinclusao_de}' AND '{$rstdtinclusao_ate}' ";
        }elseif(!empty($rstdtinclusao_de) && empty($rstdtinclusao_ate)){
            $de  = explode('/', $rstdtinclusao_de);
            $rstdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " rst.rstdtinclusao = '{$rstdtinclusao_de}' ";
        }elseif(empty($rstdtinclusao_de) && !empty($rstdtinclusao_ate)){
            $ate = explode('/', $rstdtinclusao_ate);
            $rstdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " rst.rstdtinclusao = '{$rstdtinclusao_ate}' ";
        }
        
        if(!empty($rstdtsuperacao_de) && !empty($rstdtsuperacao_ate)){
            $de  = explode('/', $rstdtsuperacao_de);
            $ate = explode('/', $rstdtsuperacao_ate);
            $rstdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $rstdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " rst.rstdtsuperacao BETWEEN '{$rstdtsuperacao_de}' AND '{$rstdtsuperacao_ate}' ";
        }elseif(!empty($rstdtsuperacao_de) && empty($rstdtsuperacao_ate)){
            $de  = explode('/', $rstdtsuperacao_de);
            $rstdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " rst.rstdtsuperacao = '{$rstdtsuperacao_de}' ";
        }elseif(empty($rstdtsuperacao_de) && !empty($rstdtsuperacao_ate)){
            $ate = explode('/', $rstdtsuperacao_ate);
            $rstdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " rst.rstdtsuperacao = '{$rstdtsuperacao_ate}' ";
        }
        
        if ( !empty($rstflressalva)){
            if($rstflressalva == 'T'){
                $where[] = " (rst.rstflressalva IN('S','N') OR rst.rstflressalva IS NULL) ";
            }else{
                $where[] = " rst.rstflressalva = '".$rstflressalva."' ";
            }
        }
        
        return $where;
    }
    
    public function getDadosListaRestricao($tipo_retorno = 'sql', $tipo = 'lista') {

        $dados = array();
        
        $where = $this->getFiltrosDadosListaRestricao();
        
        $strWhere = ( (!empty($where)) ? ' AND ' . implode(' AND ', $where) : '');
        
        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = Array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR/* ,PFLCOD_SUPERVISOR_UNIDADE */);
            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);
            $arrOrg = Array(PFLCOD_ADMINISTRADOR, PFLCOD_CADASTRADOR_INSTITUCIONAL, /*PFLCOD_CONSULTA_ESTADUAL, */ PFLCOD_CONSULTA_TIPO_DE_ENSINO, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC);
            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();

            if (possui_perfil($arrEst)) {
                $resp[]   = "urs.estuf = ende.estuf ";
                $arPflcod = array_merge($arPflcod, $arrEst);
            }

            if (possui_perfil($arrObr)) {
                $resp[]   = "urs.empid = ep.empid ";
                $arPflcod = array_merge($arPflcod, $arrObr);
            }

            if (possui_perfil($arrOrg)) {
                $resp[]   = "urs.orgid = ep.orgid ";
                $arPflcod = array_merge($arPflcod, $arrOrg);
            }

            if (possui_perfil($arrUni)) {
                $resp[]   = "urs.entid = ep.entidunidade ";
                $arPflcod = array_merge($arPflcod, $arrUni);
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid     = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid     = ($arEmpid ? $arEmpid : array(0));
                $resp[]      = " urs.entid = ep.entidunidade AND ep.empid IN('" . implode("', '", $arEmpid) . "')";
                $arPflcod[]  = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[]  = PFLCOD_CONSULTA_UNIDADE;
            }

            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    (" . implode(' OR ', $resp) . ")";
            }
        }else{
            $join['usuarioresponsabilidade'] = '';
        }

        $acao = "'<center><div style=\"width:50px;\">			
                            <img
                                    align=\"absmiddle\"
                                    src=\"/imagens/alterar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: alterarRestricao(\'' || rst.rstid || '\', \''|| obr.obrid ||'\', \''|| obr.empid ||'\');\" 
                                    title=\"Alterar\">

                                    <a href=\'/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obr.obrid || '\' target=\'_blank\'><img
                                    align=\"absmiddle\"
                                    src=\"/imagens/consultar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: abrirObra(\' ' || obr.obrid || '\' );\"
                                    title=\"Ver Obra\"></a>
                     </center></div>'  as acao,";

        $andamento = "CASE
                            WHEN doc_ri.esdid IN (" . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - h.htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Correçao\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Correção\">&nbsp;&nbsp;'
                                    END


                            WHEN doc_ri.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . " )
                                THEN

                                    CASE
                                        WHEN DATE_PART('days', NOW() - rstdtinclusao) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                    END


                            WHEN doc_ri.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . ", " . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_alerta.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'


                            WHEN doc_ri.esdid = " . ESDID_AGUARDANDO_ANALISE_FNDE . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_ativo.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Análise FNDE\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_AGUARDANDO_CORRECAO . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Correção\">&nbsp;&nbsp;'
                        END as andamento,
                         ";

        $analise = "
                        CASE
                            WHEN doc_ri.esdid IN (" . ESDID_AGUARDANDO_PROVIDENCIA . ", " . ESDID_AGUARDANDO_CORRECAO . ")
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Aguardando Providência\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_AGUARDANDO_ANALISE_FNDE . "
                                 THEN
                                 CASE
                                        WHEN DATE_PART('days', NOW() - h.htddata) >= 15
                                            THEN '<img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/0_inativo.png\"
                                                    style=\"cursor: pointer\"
                                                    title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                        ELSE
                                            '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/0_alerta.png\"
                                                style=\"cursor: pointer\"
                                                title=\"Aguardando Análise\">&nbsp;&nbsp;'
                                    END

                            WHEN doc_ri.esdid = " . ESDID_SUPERADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Superada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_JUSTIFICADA. "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_concluido2.png\"
                                    style=\"cursor: pointer\"
                                    title=\"justificada\">&nbsp;&nbsp;'

                            WHEN doc_ri.esdid = " . ESDID_CANCELADA . "
                                THEN '<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/0_inexistente.png\"
                                    style=\"cursor: pointer\"
                                    title=\"Cancelada\">&nbsp;&nbsp;'
                        END as analise,
        ";

        $colunasXls = '';
        $tipologia = '';
        if($tipo == 'xls'){
            $tipologia = "tpo.tpodsc,";
            $acao = '';
            $andamento = '';
            $analise = '';
            $colunasXls = ",
                            SUBSTRING( regexp_replace(regexp_replace(rst.rstdsc, E'<.*?>', '', 'g' ), E'&nbsp;', '', 'g') FOR 1000) as rstdsc,
                            SUBSTRING(regexp_replace(regexp_replace(rst.rstdscprovidencia, E'<.*?>', '', 'g' ), E'&nbsp;', '', 'g') FOR 1000) as rstdscprovidencia,
                            (SELECT ARRAY_TO_STRING(ARRAY(
                                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') || ';' || a.aeddscrealizada || ';' || u.usunome FROM workflow.documento d
                                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                                    WHERE d.docid = doc_ri.docid
                                    ORDER BY h.htddata ASC
                            ), ';')) as historico
                            
           ,
                
       ( SELECT ic.itcdsc from obras2.questao que  join obras2.itemcomposicaoquestao ic ON ic.itcid = que.itcid   

        where qstid = 

        ( SELECT CASE WHEN sqt.qstid IS NOT NULL THEN sqt.qstid ELSE qts.qstid END qstid
                FROM obras2.restricao r
                LEFT JOIN obras2.questaosupervisao qts ON qts.qtsid = r.qtsid
                LEFT JOIN obras2.questao q ON q.qstid = qts.qstid
                LEFT JOIN obras2.respostasubquestao rsq ON rsq.rsqid = r.rsqid
                LEFT JOIN obras2.subquestao sqt ON sqt.sqtid = rsq.sqtid
                WHERE  rst.rstid = r.rstid   )             
                )  as gerado_por_aa

                 ,
                 
        ( SELECT dq.dvqdsc from obras2.questao que  join obras2.divisaoquestao dq ON dq.dvqid = que.dvqid  

        where qstid = 

        
            ( SELECT CASE WHEN sqt.qstid IS NOT NULL THEN sqt.qstid ELSE qts.qstid END qstid
                FROM obras2.restricao r
                LEFT JOIN obras2.questaosupervisao qts ON qts.qtsid = r.qtsid
                LEFT JOIN obras2.questao q ON q.qstid = qts.qstid
                LEFT JOIN obras2.respostasubquestao rsq ON rsq.rsqid = r.rsqid
                LEFT JOIN obras2.subquestao sqt ON sqt.sqtid = rsq.sqtid
                
                WHERE  rst.rstid = r.rstid)
               
                )  as gerado_por 
            
            ";
        
         //CASE WHEN sqt.qstid IS NOT NULL THEN sqt.qstid ELSE qts.qstid END qstid
            
        }

        $sql = "  SELECT 
                        {$acao}
                        obr.obrid,
                        rst.rstid,
                        CASE WHEN rst.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
                        {$andamento}

                        CASE 
                            WHEN rst.docid IS NULL
                            THEN 'Restrição/Inconformidade sem Documento cadastrado'
                            ELSE esd_ri.esddsc 
                        END as situacao, -- Situação   
                        
                        mun.estuf as estado, -- * Estado
                        mun.mundescricao as municipio, -- * MUNICIPIO,
                        CASE
                            WHEN ep.empesfera = 'M' THEN 'Municipal'
                            WHEN ep.empesfera = 'E' THEN 'Estadual'
                            WHEN ep.empesfera = 'F' THEN 'Federal'
                            ELSE ''
                        END as esfera,
                        CASE WHEN rst.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
                        tr.tprdsc as tipo, -- * Tipo
                        $tipologia
                        '(' || obr.obrid || ') ' || obr.obrnome  as nome_obra, -- * Nome da Obra

                        esd_obr.esddsc,

                        TO_CHAR(rst.rstdtinclusao, 'DD/MM/YYYY') as data_cadastro, -- * Data Cadastro
                        usu.usunome as usucriacao, -- * Criado por
                        TO_CHAR(rst.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS previsao_providencia_dt, -- * Previsão da Providência
                        CASE WHEN rst.rstsituacao = TRUE THEN TO_CHAR(rst.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS superacao, -- * Superação
                        sup.usunome as ususuperacao, -- * Superado por
                        usuh.usunome,
                        TO_CHAR(h.htddata, 'DD/MM/YYYY') as htddata
                        {$colunasXls}
                            
                        
                            

                     FROM obras2.restricao rst

                     LEFT JOIN obras2.obras             obr ON obr.obrid  = rst.obrid
                     INNER JOIN obras2.empreendimento    ep ON ep.empid   = obr.empid
                     
                     LEFT JOIN workflow.documento        doc_ri ON doc_ri.docid  = rst.docid
                     LEFT JOIN workflow.estadodocumento  esd_ri ON esd_ri.esdid  = doc_ri.esdid
                     LEFT JOIN workflow.documento       doc_obr ON doc_obr.docid = obr.docid
                     LEFT JOIN workflow.estadodocumento esd_obr ON esd_obr.esdid = doc_obr.esdid
                     
                     LEFT JOIN obras2.programafonte          pf ON pf.prfid   = ep.prfid
                     LEFT JOIN obras2.tipoorigemobra        too ON too.tooid  = obr.tooid
                     LEFT JOIN obras2.tiporestricao          tr ON tr.tprid   = rst.tprid   AND tr.tprstatus   = 'A' 
                     LEFT JOIN obras2.faserestricao          fr ON fr.fsrid   = rst.fsrid   AND fr.fsrstatus   = 'A' 
                     LEFT JOIN obras2.tipologiaobra         tpo ON tpo.tpoid  = obr.tpoid
                     LEFT JOIN entidade.endereco           ende ON ende.endid = obr.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                     LEFT JOIN territorios.municipio        mun ON mun.muncod = ende.muncod
                     LEFT JOIN territorios.estado            uf ON mun.estuf  = uf.estuf
                     LEFT JOIN seguranca.usuario            usu ON usu.usucpf = rst.usucpf 
                     LEFT JOIN seguranca.usuario            sup ON sup.usucpf = rst.usucpfsuperacao
                     LEFT JOIN workflow.historicodocumento h ON doc_ri.hstid  = h.hstid
                     LEFT JOIN seguranca.usuario        usuh ON usuh.usucpf   = h.usucpf
                     ".$join['usuarioresponsabilidade']." 
                     WHERE rst.rststatus = 'A'
                     " . $strWhere . " 
                     
                     ORDER BY rst.rstid, obr.obrid

                     ";
                        
                        
        if($tipo_retorno == 'sql'){
            return $sql;
        }else{
            $dados = $this->carregar($sql);
            $dados = (!empty($dados)) ? $dados : array();

            return $dados;
        }

    }
    
    public function getDataEstadoAnaliseFNDE($rstid) {
        $this->carregarPorId( $rstid );
        $arr_dados = $this->getDados();        
        $docid     = $arr_dados['docid'];
        $campos    = ' hst.htddata ';
        $where     = 'AND esddestino.esdid = '.ESDID_AGUARDANDO_ANALISE_FNDE;
        $dados     = $this->getDadosTramitacaoRestricaoInconformidade($docid, $campos, $where);
        $data      = (!empty($dados['htddata'])) ? $dados['htddata'] : date('Y-m-d');
        return $data;
    }
    
    public function condicaoSuperacaoRestricoesInconformidades($rstid) {
        $dados = $this->verificaRessalvaRestricoesInconformidades($rstid);
        if(empty($dados['rstflressalva'])){
            return false;
        }
        return true;
    }
    
    public function verificaRessalvaRestricoesInconformidades($rstid, $tipo_retorno = 'bool') {
        if($rstid == null){ $rstid = $this->rstid; if($this->rstid == null){ return array(); } }
        $sql   = 'SELECT rst.rstflressalva, rst.rstobsressalva FROM obras2.restricao rst WHERE rst.rstid = '.$rstid;
        $dados = $this->pegaLinha($sql);
        $dados = (empty($dados) ? array() : $dados);
        return $dados;
    }
    
    public function posAcoesWfRestricoesInconformidades($rstid) {
        
        $this->carregarPorId( $rstid );
        $arr_dados         = $this->getDados();        
        $docid             = $arr_dados['docid'];    
        $arr_dados_tramite = $this->getDadosTramitacaoRestricaoInconformidade($docid, null, null);
        $acao              = $arr_dados_tramite['aedid'];
        
        switch ($acao) {
            case AEDID_CANCELAR:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_CANCELAR;
                break;
            case AEDID_ENCAMINHAR_PARA_ANALISE:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_ENCAMINHAR_PARA_ANALISE;
                break;
            case AEDID_CONFIRMAR_SUPERACAO:
                //Muda a flag de superado para true
                if(!$this->verificaRestricaoInconformidadeSuperada($rstid)){
                    $this->superaRestricaoInconformidade($rstid);
                }
                $_SESSION['obras2']['aedid'] = AEDID_CONFIRMAR_SUPERACAO;
                return true;
                break;
            case AEDID_DEVOLVER_PARA_CORRECAO:
                $_SESSION['obras2']['aedid'] = AEDID_DEVOLVER_PARA_CORRECAO;

                // Verifica as soolicitações de desembolso
                $s = new SolicitacaoDesembolso();
                $s->verificaTramiteSolicitacoes($this->obrid);

                break;
            case AEDID_RETORNAR_PARA_ANALISE:
                //Muda a flag de superado para false
                $this->retornaAnaliseRestricaoInconformidade($rstid);
                $_SESSION['obras2']['aedid'] = AEDID_RETORNAR_PARA_ANALISE;
                break;
            case AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA;
                break;
            case AEDID_ENVIAR_PARA_ANALISE_FNDE:
                //Do nothing
                $_SESSION['obras2']['aedid'] = AEDID_ENVIAR_PARA_ANALISE_FNDE;
                break;
            default:
                //Do nothing
                break;
        }
        $this->enviaEmailTramiteRestricoesInconformidades($rstid, $arr_dados_tramite);
    }
    
    public function superaRestricaoInconformidade($rstid){
        
        $usucpfsuperacao = $_SESSION['usucpf'];
        $rstdtsuperacao  = $this->getDataEstadoAnaliseFNDE($rstid);
        $rstsituacao     = 't';
        
        $sql = " UPDATE obras2.restricao 
                 SET usucpfsuperacao = '".$usucpfsuperacao."', 
                     rstdtsuperacao  = '".$rstdtsuperacao."', 
                     rstsituacao     = '".$rstsituacao."'
                 WHERE rstid = ".$rstid ;
        $resp = $this->carregar($sql);
        
        return $resp;
    }
    
    public function retornaAnaliseRestricaoInconformidade($rstid){
        
        $usucpfsuperacao = 'NULL';
        $rstdtsuperacao  = 'NULL';
        $rstsituacao     = 'f';
        
        $sql = " UPDATE obras2.restricao 
                 SET usucpfsuperacao = ".$usucpfsuperacao.", 
                     rstdtsuperacao  = ".$rstdtsuperacao.", 
                     rstsituacao     = '".$rstsituacao."'
                 WHERE rstid = ".$rstid ;
        $resp = $this->carregar($sql);
        
        return $resp;
    }
    
    public function recuperaTextoTramiteCorrecaoAnalise($rstid) {
        if(empty($rstid)){
            $rstid = $this->rstid;
            if(empty($rstid)){
                return '';
            }
        }
        $this->carregarPorId($rstid);
        $docid = $this->docid;
        $campos = null;
//        $where = ' AND aed.esdidorigem = '.ESDID_AGUARDANDO_CORRECAO.'  AND aed.esdiddestino = '.ESDID_AGUARDANDO_ANALISE_FNDE. ' AND aed.aedid = '.AEDID_ENVIAR_PARA_ANALISE_FNDE;
//        $where = ' AND aed.aedid = '.AEDID_ENVIAR_PARA_ANALISE_FNDE;
        $dados = $this->getDadosTramitacaoRestricaoInconformidade($docid, $campos, $where);
        if(!empty($dados)){
            $comentario = $dados['cmddsc'];
        }else{
            $comentario = '';
        }
        return $comentario;
    }
    
    
    /**
     * Recebe o docid da Restrição/Inconformidade e retorna a linha com os dados da última tramitação da Evolução
     * @param int $docid 
     * @return array $dados_tramitacao
     */
    public function getDadosTramitacaoRestricaoInconformidade($docid = null, $campos = null, $where = null){
        
        if($docid == null){
           $docid = $this->docid; 
           if($docid == null){
               return array();
           }
        }
        
        if(empty($campos)){
            $campos = " 
                        rst.obrid,
                        rst.rstid,
                        rst.usucpf as usu_cpf_cad_rst,
                        rst.docid, 
                        cmd.cmddsc,
                        
                        usu_os.usunome as usu_nome_cad_rst,
                        tpd.tpddsc, 
                        doc.docdsc,
                        esd.esdid, 
                        esd.esddsc, 
                        hst.hstid, 
                        hst.usucpf        as usu_cpf_exec_tramitacao, 
                        usu_acao.usunome  as usu_nome_exec_tramitacao, 
                        usu_acao.usuemail as usu_email_exec_tramitacao,
                        hst.htddata, 
                        aed.aedid,
                        aed.aeddscrealizada, 
                        aed.esdidorigem, 
                        aed.esdiddestino,
                        esdorigem.esddsc  as estado_de_origem, 
                        esddestino.esddsc as estado_de_destino,
                        COALESCE(to_char(hst.htddata, 'DD/MM/YYYY')) AS data_tramitacao,
                        COALESCE(to_char(hst.htddata, 'YYYY-MM-DD')) AS data_tramitacao2
                      ";
        }
        
        if(empty($where)){
            $where = '';
        }
        
        $sql = " SELECT 
                        {$campos}
                            
                 FROM obras2.restricao rst
                 
                 LEFT JOIN workflow.documento           doc ON doc.docid  = rst.docid
                 LEFT JOIN workflow.tipodocumento       tpd ON tpd.tpdid  = doc.tpdid
                 LEFT JOIN workflow.estadodocumento     esd ON esd.esdid  = doc.esdid
                 LEFT JOIN workflow.historicodocumento  hst ON hst.hstid  = doc.hstid
                 LEFT JOIN workflow.comentariodocumento cmd ON cmd.hstid  = hst.hstid
                 LEFT JOIN workflow.acaoestadodoc       aed ON aed.aedid  = hst.aedid                 
                 LEFT JOIN seguranca.usuario         usu_os ON rst.usucpf = usu_os.usucpf 
                 LEFT JOIN seguranca.usuario       usu_acao ON hst.usucpf = usu_acao.usucpf 
                 
                 LEFT JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 LEFT JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE doc.docid = ".$docid."
                   AND rst.rststatus = 'A'
                   {$where}
                 ORDER BY
                    hst.htddata DESC
                "; 
        
        $dados_tramitacao = $this->pegaLinha($sql);        
        $dados_tramitacao = ($dados_tramitacao) ? $dados_tramitacao  : array();
        
        return $dados_tramitacao;
    }
    
    /**
     * Métodos de envio de e-mail quando o Estado do WF da Restrição/Inconformidade mudar.
     */
    public function enviaEmailTramiteRestricoesInconformidades($rstid, $arr_dados_tramite) {
        // Monta o filtro de Perfis Responsáveis por cada ação do Workflow

        $gestorMI = (obraMi($this->obrid) && ($this->tprid == 13)) ? ', '.PFLCOD_EMPRESA_MI_GESTOR : '';

        switch ($arr_dados_tramite['aedid']) {
            case AEDID_CANCELAR:
                $filtro_resp = ' '.PFLCOD_SUPER_USUARIO;
                break;
            case AEDID_ENCAMINHAR_PARA_ANALISE:
                $filtro_resp = ' '.PFLCOD_GESTOR_UNIDADE.', '.PFLCOD_SUPERVISOR_UNIDADE . $gestorMI;
                break;
            case AEDID_CONFIRMAR_SUPERACAO:
                $filtro_resp = ' '.PFLCOD_GESTOR_MEC.', '.PFLCOD_SUPER_USUARIO.', '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE . $gestorMI;
                break;
            case AEDID_DEVOLVER_PARA_CORRECAO:
                $filtro_resp = ' '.PFLCOD_GESTOR_MEC.', '.PFLCOD_SUPER_USUARIO .', '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE . $gestorMI;
                break;
            case AEDID_RETORNAR_PARA_ANALISE:
                $filtro_resp = ' '.PFLCOD_SUPER_USUARIO;
                break;
            case AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA:
                $filtro_resp = ' '.PFLCOD_SUPER_USUARIO;
                break;
            case AEDID_ENVIAR_PARA_ANALISE_FNDE:
                $filtro_resp = ' '.PFLCOD_GESTOR_UNIDADE.', '.PFLCOD_SUPERVISOR_UNIDADE . $gestorMI;
                break;
            default:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE.', '.PFLCOD_GESTOR_MEC . $gestorMI;
                break;
        }
        
        $dados_responsaveis = $this->getDadosResponsaveisTramitacaoRestricaoInconformidade($rstid, $filtro_resp);
        $dados_responsaveis = (!empty($dados_responsaveis)) ? $dados_responsaveis : array();
        
        // Faz o envio do email do tramite da Restrição/Inconformidade
        if(!empty($dados_responsaveis)){
            foreach ($dados_responsaveis as $key => $usuario) {
                $destinatario = array($usuario['email']);
                $dados_email  = $this->montaCorpoEmailTamiteRestricaoInconformidade($rstid, $arr_dados_tramite, $usuario);                
                $email        = new Email();
                $email->popularDadosObjeto($dados_email);                    
                $email->salvar($destinatario);
                
                $objObras = new Obras($dados_email['obrid']);
                $situacao = $objObras ->getEstadoObraWf();
                if($situacao['esdid'] != ESDID_OBJ_CANCELADO){
                    $email->enviar();
                }
            }            
        }else{
            $destinatario = array('');
            $dados_email  = $this->montaCorpoEmailTamiteRestricaoInconformidade($rstid, $arr_dados_tramite, array());
            $email        = new Email();
            $email->popularDadosObjeto($dados_email);                    
            $email->salvar($destinatario);
            // Não enviar, apenas salvar até segunda ordem.            
        }
        
        switch ($arr_dados_tramite['aedid']) {
            
            case AEDID_CONFIRMAR_SUPERACAO:
                $this->carregarPorId($rstid);
                //Email com ressalva
                if($this->rstflressalva == 'S'){
                    if(!empty($dados_responsaveis)){
                        foreach ($dados_responsaveis as $key => $usuario) {
                            $destinatario = array($usuario['email']);
                            $dados_email = $this->montaCorpoEmailTamiteRestricaoInconformidadeSuperadoComRessalva($rstid, $arr_dados_tramite, $dados_responsaveis);
                            $email        = new Email();
                            $email->popularDadosObjeto($dados_email);                    
                            $email->salvar($destinatario);
                            
                            $objObras = new Obras($dados_email['obrid']);
                            $situacao = $objObras ->getEstadoObraWf();
                            if($situacao['esdid'] != ESDID_OBJ_CANCELADO){
                                $email->enviar();
                            }
                        }
                    }else{
                        $destinatario = array('');
                        $dados_email  = $this->montaCorpoEmailTamiteRestricaoInconformidadeSuperadoComRessalva($rstid, $arr_dados_tramite, $dados_responsaveis);
                        $email        = new Email();
                        $email->popularDadosObjeto($dados_email);                    
                        $email->salvar($destinatario);
                    }
                }
                //Email sem ressalva
                else{
                    if(!empty($dados_responsaveis)){
                        foreach ($dados_responsaveis as $key => $usuario) {
                            $destinatario = array($usuario['email']);
                            $dados_email = $this->montaCorpoEmailTamiteRestricaoInconformidadeSuperadoSemRessalva($rstid, $arr_dados_tramite, $dados_responsaveis);
                            $email        = new Email();
                            $email->popularDadosObjeto($dados_email);                    
                            $email->salvar($destinatario);
                            
                            $objObras = new Obras($dados_email['obrid']);
                            $situacao = $objObras ->getEstadoObraWf();
                            if($situacao['esdid'] != ESDID_OBJ_CANCELADO){
                                $email->enviar();
                            }
                        }
                    }else{
                        $destinatario = array('');
                        $dados_email = $this->montaCorpoEmailTamiteRestricaoInconformidadeSuperadoSemRessalva($rstid, $arr_dados_tramite, $dados_responsaveis);
                        $email        = new Email();
                        $email->popularDadosObjeto($dados_email);                    
                        $email->salvar($destinatario);
                    }
                }
                break;
            case AEDID_DEVOLVER_PARA_CORRECAO:
                //email 1
                if(!empty($dados_responsaveis)){
                    foreach ($dados_responsaveis as $key => $usuario) {
                        $destinatario = array($usuario['email']);
                        $dados_email = $this->montaCorpoEmailTamiteRestricaoInconformidadeDevolvendoParaCorrecao($rstid, $arr_dados_tramite, $dados_responsaveis);
                        $email        = new Email();
                        $email->popularDadosObjeto($dados_email);                    
                        $email->salvar($destinatario);
                        
                        $objObras = new Obras($dados_email['obrid']);
                        $situacao = $objObras ->getEstadoObraWf();
                        if($situacao['esdid'] != ESDID_OBJ_CANCELADO){
                            $email->enviar();
                        }
                    }
                }else{
                    $destinatario = array('');
                    $dados_email = $this->montaCorpoEmailTamiteRestricaoInconformidadeDevolvendoParaCorrecao($rstid, $arr_dados_tramite, $dados_responsaveis);
                    $email        = new Email();
                    $email->popularDadosObjeto($dados_email);                    
                    $email->salvar($destinatario);
                }
                
                break;
            
            default:
                break;
        }
        
        return true;
    }
    /**
     * 
     * @param int $rstid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     */
    public function montaCorpoEmailTamiteRestricaoInconformidadeDevolvendoParaCorrecao($rstid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosSimplificadoRestricaoInconformidade($rstid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        $email = new Email();
        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
        
        $hstid = $array_dados['hstid'];
        $sql = 'SELECT cmddsc FROM workflow.comentariodocumento WHERE hstid = '.$hstid;
        $obs = $this->pegaUm($sql);
        $obs = (empty($obs)) ? '' : $obs;
        
        $html = ' 
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                        <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        '.$data.'
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px;" colspan="2">
                                        <b> RESTRIÇÕES E INCONFORMIDADES </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: Análise dos documentos encaminhados (Restrições/Inconformidades)
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Obra '.$nome_obra.' 
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Prezado Fiscal,
                                        Após análise dos documentos apresentados para superação das restrições/inconformidades, informamos que os mesmos não foram suficientes e solicitamos atenção e presteza no atendimento ao indicado no campo observações redigido pelo nosso corpo técnico, conforme abaixo transcrito:
                                    </td>
                                </tr>
                                <tr>
                                    <td colspan="2"> <b>'.$obs.'</b> </td>
                                </tr>   
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
                ';
        
            $assunto            = "Análise dos documentos encaminhados (Restrições/Inconformidades)";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 13,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }
    /**
     * 
     * @param int $rstid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     */
    public function montaCorpoEmailTamiteRestricaoInconformidadeSuperadoSemRessalva($rstid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosSimplificadoRestricaoInconformidade($rstid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        $email = new Email();
        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
        
        $html = ' 
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                        <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        '.$data.'
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px;" colspan="2">
                                        <b> RESTRIÇÕES E INCONFORMIDADES </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: Análise dos documentos encaminhados (Restrições/Inconformidades)
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Obra '.$nome_obra.' 
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Prezado Fiscal,
                                        A restrição/inconformidade foi superada pelo monitoramento de obras do FNDE.
                                    </td>
                                </tr>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
                ';
        
            $assunto            = "Análise dos documentos encaminhados (Restrições/Inconformidades)";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 13,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }
    /**
     * 
     * @param int $rstid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     */
    public function montaCorpoEmailTamiteRestricaoInconformidadeSuperadoComRessalva($rstid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosSimplificadoRestricaoInconformidade($rstid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        $email = new Email();
        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
        
        $html = ' 
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                        <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        '.$data.'
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px;" colspan="2">
                                        <b> RESTRIÇÕES E INCONFORMIDADES </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: Análise dos documentos encaminhados (Restrições/Inconformidades)
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Obra '.$nome_obra.' 
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Prezado Fiscal,
                                        A restrição/inconformidade foi analisada e superada com ressalvas, conforme descrito abaixo:
                                    </td>
                                </tr>
                                <tr>
                                    <td colspan="2"> <b>'.$this->rstobsressalva.'</b> </td>
                                </tr>    
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
                ';
        
            $assunto            = "Análise dos documentos encaminhados (Restrições/Inconformidades)";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 13,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }
    /**
     * 
     * @param int $rstid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     * @example $dados = array( 'usucpf'               => $_SESSION['usucpf'], 
                                'emlconteudo'          => $conteudo,
                                'emlassunto'           => $assunto, 
                                'temid'                => 13,
                                'emlregistroatividade' => true,
                                'obrid'                => $obrid
                              );
     */
    public function montaCorpoEmailTamiteRestricaoInconformidade($rstid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosSimplificadoRestricaoInconformidade($rstid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        
        $data_tramitacao    = $array_dados['data_tramitacao'];
        $nome_tramitador    = $array_dados['usu_nome_exec_tramitacao'];
        $email_tramitador   = $array_dados['usu_email_exec_tramitacao'];
        $estado_atual_ri    = $array_dados['estado_de_destino'];
        $estado_anterior_ri = $array_dados['estado_de_origem'];
        $acao_realizada     = $array_dados['aeddscrealizada'];

        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
        
        if(trim($this->rstobsressalva) != '' && $_SESSION['obras2']['aedid'] == AEDID_CONFIRMAR_SUPERACAO){
            unset($_SESSION['obras2']['aedid']);
            $html_ressalva = ' 
                                <tr>
                                    <td>A Restrição/Inconformidade foi Superada, porém com a seguinte Ressalva:</td>
                                    <td> <b>'.$this->rstobsressalva.'</b></td>
                                </tr>     
                             ';
        }else{
            $html_ressalva = '';
        }
        $email = new Email();
        $html = ' 
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                        <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        '.$data.'
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px;" colspan="2">
                                        <b> RESTRIÇÕES E INCONFORMIDADES </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: SIMEC - RESTRIÇÕES E INCONFORMIDADES - ID '.$rstid.' - Tramitada
                                    </td>
                                </tr>

                                <tr>
                                    <td>Nome do Responsável:</td>
                                    <td>'.$nome_responsavel.'</td>
                                </tr>
                                <tr>
                                    <td>CPF do Responsável:</td>
                                    <td>'.$cpf_responsavel.'</td>
                                </tr>
                                <tr>
                                    <td>Obra:</td>
                                    <td>'.$nome_obra.'</td>
                                </tr>                            
                                <tr>
                                    <td>Estado Atual:</td>
                                    <td><b> '.$estado_atual_ri.' </b></td>
                                </tr>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td>Estado Anterior:</td>
                                    <td><b> '.$estado_anterior_ri.' </b></td>
                                </tr>
                                <tr>
                                    <td>Data da Tramitação:</td>
                                    <td>'.$data_tramitacao.'</td>
                                </tr>
                                <tr>
                                    <td>Quem tramitou:</td>
                                    <td> '.$nome_tramitador.' - ('.$email_tramitador.')</td>
                                </tr>
                                <tr>
                                    <td>Ação realizada:</td>
                                    <td> <b>'.$acao_realizada.'</b></td>
                                </tr>
                                    '.$html_ressalva.' 
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
                ';
        
            $assunto            = SIGLA_SISTEMA. " - RESTRIÇÕES E INCONFORMIDADES - ID ".$rstid." - Tramitada";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 13,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }
    
    public function getDadosSimplificadoRestricaoInconformidade($rstid, $arr_campos = array()) {
        if(empty($rstid)){ return array(); }
        if(empty($arr_campos)){
            $campos = 'rst.*, d.*, ed.*, o.empid';
        }else{
            $campos = implode(', ', $arr_campos);
        }
        $sql = 'SELECT '.$campos.'  
                FROM obras2.restricao rst
                INNER JOIN obras2.obras             o ON rst.obrid  = o.obrid
                LEFT JOIN seguranca.usuario        su ON rst.usucpf = su.usucpf
                LEFT JOIN workflow.documento 	    d ON d.docid    = rst.docid --AND tpdid = 105
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid   = d.esdid
                LEFT JOIN obras2.tiporestricao     tr ON tr.tprid   = rst.tprid   AND tr.tprstatus   = \'A\' 
                LEFT JOIN obras2.faserestricao     fr ON fr.fsrid   = rst.fsrid   AND fr.fsrstatus   = \'A\' 
                LEFT JOIN obras2.tipologiaobra    tpo ON tpo.tpoid  = o.tpoid
                WHERE rst.rststatus = \'A\'
                  AND rst.rstid     = '.$rstid;
        $arr_dados = $this->pegaLinha($sql);
        $arr_dados = ($arr_dados) ? $arr_dados : array();
        return $arr_dados;
    
    }

    public function getDadosResponsaveisTramitacaoRestricaoInconformidade($rstid, $filtro_resp){

        $arr = $this->getDadosSimplificadoRestricaoInconformidade($rstid);
        
        if(!empty($arr)){
            $empid = $arr['empid'];            
        }else{
            return array();
        }
        
        if(trim($filtro_resp) == ''){
            return array();            
        }

        $perfils = array_map('trim', explode(',', $filtro_resp));
        $responsaveisMI = array();
        if (($key = array_search(PFLCOD_EMPRESA_MI_GESTOR, $perfils)) !== FALSE) {
            $empresaMI = new EmpresaMi();
            $responsaveisMI = $empresaMI->pegaReponsaveis($this->obrid, PFLCOD_EMPRESA_MI_GESTOR);
            unset($perfils[$key]);
        }
        $filtro_resp = implode(', ', $perfils);

        $filtro_sql_resp = " AND ur.pflcod IN (".$filtro_resp.")";

        $sql_responsaveis = "SELECT DISTINCT
                                         su.usucpf   as cpf,
                                         su.usunome  as nome,
                                         su.usuemail as email
                             FROM seguranca.usuario su
                             INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A'
                             WHERE
                                     ur.empid = {$empid}
                                     {$filtro_sql_resp}
                             ORDER BY
                                     su.usunome";

        $dados_responsaveis = $this->carregar($sql_responsaveis);
        $dados_responsaveis = (is_array($dados_responsaveis) && count($dados_responsaveis) > 0) ? $dados_responsaveis : array();
        $dados_responsaveis = array_merge($dados_responsaveis , $responsaveisMI);

        return $dados_responsaveis;
    }
    
    public function getDadosUsuarioCpf($cpf = NULL) {
        if($cpf == NULL){
            $cpf = $_SESSION['usucpf'];
        }
        $cpf = (string)$cpf;
        $sql = "SELECT usunome, usucpf, usuemail
                FROM seguranca.usuario 
                WHERE usucpf = '".$cpf."'";
        $dados_usuario = $this->pegaLinha($sql);
        $dados_usuario = ($dados_usuario) ? $dados_usuario : array();
        return $dados_usuario;
    }
    
    public function getListaArquivosRestricaoInconformidade($rstid) {
        
        if(empty($rstid)){ return array(); }
        
        //Colocar aqui a regra de exclusão
        $perfil_exclusao = true;
        $veri_sup = $this->verificaRestricaoInconformidadeSuperada($rstid);
        if($perfil_exclusao == true && $veri_sup == false){
    		$img_excluir = "<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir_2.gif\"
                                    width=\"19px\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    id=\"' || ra.arqid || '\"
                                    class=\"excluir\"
                                    title=\"Excluir Arquivo\">
			    	</center>";
    	}else{
    		$img_excluir = "</center>";
    	}
        
        $sql = "SELECT
                        '<center>
                        <img
                            align=\"absmiddle\"
                            src=\"/imagens/icone_lupa.png\"
                            style=\"cursor: pointer\"
                            id=\"' || ra.arqid || '\"
                            class=\"download\"
                            title=\"Baixar Arquivo\">
                            $img_excluir
                        ' as acao,
                        ta.tpadesc,
                        COALESCE(ra.rardesc, '')  as descricao,
                        '<a href=\"javascript:void(0);\" 
                                id=\"' || ra.arqid || '\" 
                                class=\"download\" 
                                title=\"Baixar Arquivo\">' 
                                || a.arqnome || '.' || a.arqextensao || 
                        '</a>' as nome_arquivo,
                        to_char(rardtinclusao,'DD/MM/YYYY') AS data,
                        usu.usunome
                FROM
                        obras2.restricao_arquivo ra
                JOIN obras2.tipoarquivo ta ON ta.tpaid   = ra.tpaid
                JOIN public.arquivo      a ON a.arqid    = ra.arqid
                JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
                WHERE
                      ra.rstid = ".$rstid."  
                  AND ra.rarstatus = 'A'
                ORDER BY
                        2";
        
        try{
            $dados = $this->carregar($sql);
        } catch (Exception $ex) {
            $dados = array();
        }
        
        $dados = (empty($dados)) ? array() : $dados ;
        
        return $dados;
        
    }
    
    public function getDadosArquivosRestricaoInconformidade($rstid) {

        if(empty($rstid)){ return array(); }
        
        $sql          = 'SELECT * FROM obras2.restricao_arquivo WHERE rstid = '.$rstid;
        $arquivos     = $db->carregar($sql);    
        $arquivos     = (is_array($arquivos)) ? $arquivos : array();    
        $arr_arquivos = array();    
        foreach ($arquivos as $key => $arq) {
            $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arq['arqid']. " AND arqstatus = 'A'";
            $arquivo = $db->pegaLinha($sql);
            if(is_array($arquivo)){
                $filename = $arquivo['arqnome'].'.'.$arquivo['arqextensao'];        
                $arr_arquivos[] = array( 'arqid'   => $arq['arqid'],
                                         'arqnome' => $filename,
                                         'arqdesc' => trim($arq['rardesc'])
                                       );
            }        
        }    
        return $arr_arquivos;
        
    }
    
    public function excluirArquivosRestricaoInconformidade($arqid) {
        $erro = false;
        
        $sql = "UPDATE obras2.restricao_arquivo SET rarstatus = 'I' WHERE arqid = ".$arqid;
        try{
            $resp = $this->executar($sql);
        } catch (Exception $ex) {
            $erro = true;
        }
        
        $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid;
        try{
            $resp = $this->executar($sql);
        } catch (Exception $ex) {
            $erro = true;
        }
        
        if($erro){
            $this->rollback();
        }else{
            $this->commit();
        }
        
        return $resp;
    }
    
    public function gravaArquivosRestricaoInconformidade($rstid, $post = array()){
        
        $erro     = false;
        $msg_erro = '';
        
        if(empty($post)){ $post = $_POST; }
        
        $arquivo = $_FILES['arquivo'];
        
        $qtd_arquivos = count($arquivo['name']);
        
        for($c=0;$c<$qtd_arquivos;$c++){
            
            $arquivosparts    = explode(".", $arquivo["name"][$c]);
            $arqnome          = current($arquivosparts);
            $arqextensao      = end($arquivosparts);
            $descricaoArquivo = $post['rardesc'][$c];

            $sql = "INSERT INTO public.arquivo (arqnome,
                                                arqextensao,
                                                arqdescricao,
                                                arqtipo,
                                                arqtamanho,
                                                arqdata,
                                                arqhora,
                                                usucpf,
                                                sisid,
                                                arqstatus)
                            VALUES( '".simec_addslashes($arqnome)."',
                                    '".$arqextensao."',
                                    '".$descricaoArquivo."',
                                    '".$arquivo["type"][$c]."',
                                    '".$arquivo["size"][$c]."',
                                    '".date('Y-m-d')."',
                                    '".date('H:i:s')."',
                                    '".$_SESSION["usucpf"]."',
                                     ".$_SESSION["sisid"].",
                                    'A') 
                            RETURNING arqid;";
            try{
                $arqid = $this->pegaUm($sql);
            } catch (Exception $ex) {
                $arqid     = false;
                $erro      = true;
                $msg_erro .= 'Erro ao cadastrar o arquivo público.'.$ex->getMessage();
            }
            
            if($arqid){
                $nomeEsquema = 'obras2';
                $caminhoArquivo = APPRAIZ."arquivos/".$nomeEsquema."/".floor($arqid/1000).'/'.$arqid;
                
                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema)) {
                        mkdir(APPRAIZ."arquivos/".$nomeEsquema, 0777);
                }
                if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000))) {
                    $pasta = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000);
                        $resp = mkdir($pasta, 0777, true);
                }
                
                switch($arquivo["type"][$c]) {
                    case 'image/jpeg':
                        ini_set("memory_limit", "128M");
                        list($width, $height) = getimagesize($arquivo["tmp_name"][$c]);
                        $original_x = $width;
                        $original_y = $height;
                        // se a largura for maior que altura
                        if($original_x > $original_y) {
                            if($original_x != 0){
                                $porcentagem = (100 * 640) / $original_x;
                            }else{
                                $porcentagem = 0;
                            }
                        }else {
                            if($original_y != 0){
                                $porcentagem = (100 * 480) / $original_y;  
                            }else{
                                $porcentagem = 0;
                            }
                        }

                        $tamanho_x = $original_x * ($porcentagem / 100);
                        $tamanho_y = $original_y * ($porcentagem / 100);
                        $image_p   = imagecreatetruecolor($tamanho_x, $tamanho_y);
                        $image     = imagecreatefromjpeg($arquivo['tmp_name'][$c]);

                        imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                        imagejpeg($image_p, $caminhoArquivo, 100);
                        ImageDestroy($image_p);//Clean-up memory
                        ImageDestroy($image);//Clean-up memory

                        break;
                    default:
                        if ( !move_uploaded_file( $arquivo["tmp_name"][$c], $caminhoArquivo) ) {
                            $erro      = true;
                            $msg_erro .= ' \n Problemas no envio do arquivo '.$c.' da lista.';
                        }
                }
                
                // 21 - Outros
                $tpaid = (empty($post['tpaid'][$c])) ? 21 : $post['tpaid'][$c];
                
                $sql = "INSERT INTO obras2.restricao_arquivo (rstid,
                                                              arqid,
                                                              tpaid,
                                                              rardesc,
                                                              rardata,
                                                              rarstatus,
                                                              rardtinclusao
                                                             )
                                                    VALUES( $rstid,
                                                            $arqid,
                                                            ".$tpaid.",
                                                           '".$descricaoArquivo."',
                                                            now(),
                                                            'A',
                                                            now()
                                                          ) 
                                                    RETURNING rarid;";
                
                try{
                    $rarid = $this->pegaUm($sql);
                } catch (Exception $ex) {
                    $rarid     = false;
                    $erro      = true;
                    $msg_erro .= 'Erro ao cadastrar o arquivo da restrição.'.$ex->getMessage();
                }
            }
        }
        
        if($erro){
            $this->rollback();
            $msg = $msg_erro;
        }else{
            $this->commit();
            $msg = ' Arquivos cadastrados com sucesso. ';
        }
        
        return array('erro'=>$erro, 'msg'=>$msg);

    }
    
    /**
     * Os 7 métodos abaixo são utilizados na ação do arquivo /simec/seguranca/www/scripts_exec/obras2_atualizaDocidRestricaoInconformidade.php
     * A atualização dos IDs foi executada no dia 05/05/2014 - 16:30
     * 
     * @return type
     */
    public function getQtdRestricoes() {
        $sqlA = "SELECT count(*) 
                FROM obras2.restricao 
                WHERE rststatus = 'A' ";
        $rA = $this->pegaUm($sqlA);
        $sqlI = "SELECT count(*) 
                FROM obras2.restricao 
                WHERE rststatus = 'I' ";
        $rI = $this->pegaUm($sqlI);
        
        return array('ativas'=> $rA, 'inativas'=> $rI);
    }    
    
    public function atualizaTodosOsDocids() {
        /**
         * 1 - Recupera todos os rstid ativos e sem DOCID
         * 2 - Foreach - Atualiza um a um
         * 3 - Cria o DOCID
         * 4 - Atualiza na tabela de restrições
         * 5 - Verifica superação
         * 6 - Atualiza os docs superados para o estado de superado
         */
        
        $sql1 = "SELECT rstid 
                 FROM obras2.restricao 
                 WHERE rststatus = 'A' 
                   AND docid IS NULL
                 ORDER BY rstid 
                 LIMIT 10000";
        
        $resInc = $this->carregar($sql1);
        $resInc = (empty($resInc)) ? array() : $resInc;
        $erro   = array();
        
        foreach ($resInc as $key => $value) {
            $rstid = $value['rstid'];
            $resp = $this->atualizaDocidNullRetricao($rstid);
            
            if($resp == false){
                $erro[] = array('rstid' => $rstid, 'erro' => true, 'msg' => 'Erro ao atualizar o DOCID!');
            }else{
                $erro[] = array('rstid' => $rstid, 'erro' => false, 'msg' => 'DOCID atualizado com sucesso!');
            }
        }
        
        if(count($erro) > 0){
            $this->rollback();
        }else{
            $this->commit();
        }
        
        return $erro;
    }
    
    /**
     * Método usado para atualizar as Restrições que foram cadastradas sem DOCID. 
     * Também usado em outras partes do sistema além do arquivo /simec/seguranca/www/scripts_exec/obras2_atualizaDocidRestricaoInconformidade.php
     * 
     * @param type $rstid
     * @param type $ret_docid
     * @return boolean
     */
    public function atualizaDocidNullRetricao($rstid = null, $ret_docid = null){
        if(empty($rstid)){ $rstid = $this->rstid; if(empty($rstid)){ return false; } }
        
        $sql = 'SELECT rst.docid FROM obras2.restricao rst WHERE rst.rstid = '.$rstid;
        $docid_teste = $this->pegaUm($sql);
        if(!empty($docid_teste)){
            return $docid_teste;
        }
        
        $docid = $this->criarDocidRestricaoInconformidade($rstid);
        $resp  = $this->atualizaDocidRestricaoInconformidade($docid, $rstid);
        /**
         * Verificar superação e atualizar o docid para o estado de superado, caso necessário.
         */
        if($this->verificaRestricaoInconformidadeSuperada($rstid) && $resp){
            $resp = $this->atualizaDocidRetricaoParaSuperado($rstid, $docid);
            $this->posAcoesWfRestricoesInconformidades($rstid);
        }
        
        if($ret_docid == null){
            return $resp;
        }elseif($ret_docid == null && $resp){
            return $docid;
        }else{
            return false;
        }
    }

    public function criarDocidRestricaoInconformidade($rstid){
        if (empty($rstid)) {
            $rstid = $this->rstid;
            if(empty($rstid)){
                return false;
            }
        }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo de Restrição/Inconformidade do módulo Obras II - rstid: " . $rstid;
        $docid = wf_cadastrarDocumento(TPDID_RESTRICAO_INCONFORMIDADE, $docdsc);
        return $docid;
    }

    public function atualizaDocidRestricaoInconformidade($docid, $rstid) {        
        if(empty($docid) || empty($rstid)){
            return false;
        }        
        $sql = 'UPDATE obras2.restricao SET docid = ' . $docid . ' WHERE rstid = ' . $rstid;
        try {
            $resp = $this->executar($sql);
            $this->commit();
        } catch (Exception $exc) {
            $resp = false;
        }
        return $resp;
    }
    
    /**
     * Método criado para autalizar as Restrições já superadas antes da implementação do Workflow nas Restrições
     * @param type $rstid
     * @param type $docid
     * @return boolean
     */
    public function atualizaDocidRetricaoParaSuperado($rstid = null, $docid = null){
        
        if(empty($rstid)){ $rstid = $this->rstid; if(empty($rstid)){ return false; } }
        if(empty($docid)){ $docid = $this->docid; if(empty($docid)){ return false; } }
        
        $aedid  = AEDID_CONFIRMAR_SUPERACAO;
        $esdid  = ESDID_SUPERADA;
        $cmddsc = 'Restrição/Inconformidade superada de forma automatizada de acordo com os dados legado de antes da implementação do Workflow nas Restrições.';
        
        // cria log no histórico
	$sqlHistorico = " insert into workflow.historicodocumento
                          ( aedid, docid, usucpf, htddata )
                          values ( " . $aedid . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', now() )
                          returning hstid ";
        
	$hstid = (integer) $this->pegaUm( $sqlHistorico );
        
	if (!$hstid){
            $this->rollback();
            return false;
	}
	
	// cria comentario para essa ação automatizada	
        $sqlComentario = " insert into workflow.comentariodocumento
                           ( docid, hstid, cmddsc, cmddata, cmdstatus )
                           values ( " . $docid . ", " . $hstid . ", '" . addslashes($cmddsc) . "', now(), 'A' )";
        
        if (!$this->executar( $sqlComentario )){
            $this->rollback();
            return false;
        }
	
	// atualiza documento
	$sqlDocumento = " update workflow.documento
                          set esdid   = " . $esdid . "
                          where docid = " . $docid;
        
        if (!$this->executar( $sqlDocumento )){
            $this->rollback();
            return false;
	}
        return true;
    }

    public function verificaRestricaoInconformidadeSuperada($rstid){
        $sql = 'SELECT rst.rstsituacao FROM obras2.restricao rst WHERE rst.rstid = '.$rstid;
        $flag_superada = ($this->pegaUm($sql) == 't') ? true : false ;
        return $flag_superada;
    }
    
    /**
     * Método usado no dia 05/08/2014 para remover as TAGs HTML dos registros das Restrições e Inconformidades nos campos rstdsc, rstdscprovidencia, rstobsressalva
     * @param type $executa
     * @return int
     */
    public function atualizarRestricaoInconformidadeRemovendoTagsHtmlDosCampos($executa = false) {
        
        $sql   = " SELECT --*
                    rstid,
                    rstdsc,
                    rstdscprovidencia,
                    rstobsressalva
                   FROM obras2.restricao r 
                   WHERE r.rststatus = 'A' 
                 " ;
        
        $dados = $this->carregar($sql);
        
        $dados_atualizar = array();
        
        if(!empty($dados)){
            foreach ($dados as $key => $value) {
                if( ($value['rstdsc'] != strip_tags($value['rstdsc'])) || 
                    ($value['rstdscprovidencia'] != strip_tags($value['rstdscprovidencia'])) || 
                    ($value['rstobsressalva'] != strip_tags($value['rstobsressalva']))     ) {
                    
                    $dados_atualizar[] = array( 'rstid'             => addslashes(strip_tags($value['rstid']            , '<strong><p><br>')),
                                                'rstdsc'            => addslashes(strip_tags($value['rstdsc']           , '<strong><p><br>')),
                                                'rstdscprovidencia' => addslashes(strip_tags($value['rstdscprovidencia'], '<strong><p><br>')),
                                                'rstobsressalva'    => addslashes(strip_tags($value['rstobsressalva']   , '<strong><p><br>')));
                }
            }
        }
        
        if($executa && !empty($dados_atualizar)){            
            $arr_retorno = array();            
            $count_ok   = 0;
            $count_erro = 0;
            
            foreach ($dados_atualizar as $k => $v) {
                
                $sql_update = " UPDATE obras2.restricao SET
                                    rstdsc            = '".$v['rstdsc']."',
                                    rstdscprovidencia = '".$v['rstdscprovidencia']."',
                                    rstobsressalva    = '".$v['rstobsressalva']."'
                                WHERE rstid = ".$v['rstid']."
                              ";
                
                try{
                    $this->executar($sql_update);
                    $count_ok++;
                    $arr_retorno[] = array('erro' => false, 'msg' => 'SQL Ok.', 'rstid' => $v['rstid'], 'sql' => $sql_update);
                            
                } catch (Exception $ex) {
                    $count_erro++;
                    $arr_retorno[] = array('erro' => true, 'msg' => $ex->getMessage(), 'rstid' => $v['rstid'], 'sql' => $sql_update);
                }
                
            }
            
            $arr_retorno['qtd_erros'] = $count_erro;
            $arr_retorno['qtd_ok']    = $count_ok;
            
            if($count_erro > 0){
                $this->rollback();
            }else{
                $this->commit();
            }
            
            return $arr_retorno;
            
        }else{
            return $dados_atualizar;
        }
    }

    public function verificaExistenciaRestricao($rsqid,$qtsid,$obrid)
    {
        $superada = ESDID_SUPERADA;
        $cancelada = ESDID_CANCELADA;
        $justificada = ESDID_JUSTIFICADA;
        if ($rsqid) {
            $sql = <<<DML
                SELECT
                    r.rstid
                FROM {$this->stNomeTabela} r
                INNER JOIN workflow.documento doc ON r.docid = doc.docid
                WHERE r.rsqid = $rsqid
                    AND r.obrid = $obrid
                    AND r.rststatus = 'A'
                    AND doc.esdid NOT IN ($superada,$cancelada,$justificada)
DML;
            return $this->pegaUm($sql);
        } else if ($qtsid) {
            $sql = <<<DML
                SELECT
                    r.rstid
                FROM {$this->stNomeTabela} r
                INNER JOIN workflow.documento doc ON r.docid = doc.docid
                WHERE r.qtsid = $qtsid
                    AND r.obrid = $obrid
                    AND r.rststatus = 'A'
                    AND doc.esdid NOT IN ($superada,$cancelada,$justificada)
DML;
            return $this->pegaUm($sql);
        }
        return false;
    }
}