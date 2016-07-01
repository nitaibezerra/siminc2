<?php

class DemandaEntrega extends Modelo
{

    // Tipos de entrega
    const K_TIPO_TOTAL = 'ET';
    const K_TIPO_PARCIAL = 'EP';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.demandaentrega";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dmeid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dmeid' => null,
        'dmdid' => null,
        'dmedsc' => null,
        'dmestatus' => null,
        'dmetipo' => null,
        'usucpfinclusao' => null,
        'dmedtinclusao' => null,
        'usucpfalteracao' => null,
        'dmedtalteracao' => null,
        'usucpfinativacao' => null,
        'dmedtinativacao' => null,
        'esdid' => null,
        'docid' => null,
		'dmerelocorrencia'=> null
    );

    public function getTipos()
    {
        $dados = array(
            array(
                'codigo' => self::K_TIPO_TOTAL,
                'descricao' => 'Entrega Total'
            ),
            array(
                'codigo' => self::K_TIPO_PARCIAL,
                'descricao' => 'Entrega Parcial'
            ),
        );
        return $this->getOptions($dados, array(), 'dmetipo');
    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null)
    {
        $html = '';
        $selected = '';

        if (isset ($htmlOptions ['prompt'])) {
            $html .= '<option value="">' . strtr($htmlOptions ['prompt'], array(
                    '<' => '&lt;',
                    '>' => '&gt;'
                )) . "</option>\n";
        }

        if ($dados) {
            foreach ($dados as $data) {
                if ($idCampo) {
                    $selected = ($data ['codigo'] === $this->$idCampo ? "selected='true' " : "");
                }
                $html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data ['codigo'] . ">  " . simec_htmlentities($data ['descricao']) . " </option> ";
            }
        }
        return $html;
    }

	public function getLista($modelDemanda = null){
        $esdidAprovada = ESD_ENTREGA_APROVADA;
        $esdidEmElaboracao = ESD_ENTREGA_EM_ELABORACAO;
        $esdidAguardando = ESD_ENTREGA_AGUARDANDO_APROVACAO;
        $esdidNucleoJuridico = ESD_DEMANDA_NUCLEO_JURIDICO;
		$where = array();
		$whereSQl = '';

		$esdid = (int)$_POST['pendecia'];
		$dt_inicio = formata_data_sql($_POST['dt_inicio']);
		$dt_fim = formata_data_sql($_POST['dt_fim']);

        $usucpf = $_SESSION['usucpf'];
        $sisId = SIS_DEMANDASFIES;

        if ($_POST ['action'] == 'pesquisar') {

            $area = (int)$_POST ['area'];
            if(!empty($area)){
                $where[] = " (ed.esdid = {$area})";
            }

			if ( !empty($esdid) ) {
				$where [] = " est_entrega.esdid = {$esdid} ";
			}

			if(!empty($dt_inicio)){
				$where[] = " hd.htddata >= '{$dt_inicio} 00:00:00' ";
			}

			if(!empty($dt_fim)){
				$where[] = " hd.htddata <=  '{$dt_fim} 23:00:00' ";
			}

			if($this->dmetipo){
				$where[] = " de.dmetipo = '{$this->dmetipo}' ";
			}
			if($this->dmerelocorrencia){
				$where[] = " de.dmerelocorrencia = '{$this->dmerelocorrencia}' ";
			}
			if(isset($_POST['acjid'])  && !empty( $_POST['acjid'] ) ){
				$esdid = (int)$_POST['acjid'];
				$where[] = " acj.acjid = $esdid ";
			}

        }

		if($modelDemanda){
			$where[] = " de.dmdid = {$modelDemanda->dmdid}";
		}

		$where = array_filter($where);
		if (!empty ($where)) {
			$where = implode(' AND ', $where);
			$whereSQl .= ' AND ' . $where;
		}

        $perfis = pegaPerfilGeral();
        $perfisSql = implode(',', $perfis);
        $sql = "SELECT  DISTINCT (de.dmeid),
						de.dmdid,
						acj.acjnome,
						de.dmedsc, to_char(de.dmedtinclusao, 'DD/MM/YYYY HH24:mi:ss'),
						usunome,
						ed.esddsc,
						CASE WHEN de.dmetipo = 'ET' THEN 'Entrega Total'
							WHEN de.dmetipo = 'EP' THEN 'Entrega Parcial'
					   	END as dmetipo,
					   	de.usucpfinclusao,
					   	CASE WHEN de.dmerelocorrencia = 'f' THEN 'NÃO'
							WHEN de.dmerelocorrencia = 't' THEN 'SIM'
					   	END as dmerelocorrencia,
					   	de.dmedtinclusao
					   	,est_entrega.esddsc as desc
					   	,to_char(hd.htddata, 'DD/MM/YYYY') as htddata
                        FROM demandasfies.demandaentrega de
                                INNER JOIN seguranca.usuario u on de.usucpfinclusao = u.usucpf
                                INNER JOIN workflow.estadodocumento ed on ed.esdid = de.esdid
                                INNER JOIN workflow.documento doc ON doc.docid = de.docid

                                LEFT JOIN workflow.historicodocumento hd ON hd.hstid = doc.hstid
								LEFT JOIN workflow.historicodocumento hdoc ON hdoc.docid = doc.docid

                                INNER JOIN workflow.estadodocumento est_entrega on est_entrega.esdid = doc.esdid
                                INNER JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                                INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod  AND p.sisid =  {$sisId}
                                INNER JOIN demandasfies.demanda d on de.dmdid = d.dmdid
                                INNER  JOIN demandasfies.acaojudicial acj on acj.acjid = d.acjid
								INNER JOIN workflow.documento doc_entrega on de.docid = doc_entrega.docid
								INNER JOIN workflow.estadodocumento estadodocumento_entrega on estadodocumento_entrega.esdid = doc_entrega.esdid
                        WHERE  1 = 1
                              	{$whereSQl}
                          AND de.dmestatus = 'A'
                        ORDER BY de.dmedtinclusao desc";
//		ver($sql, d);
        $listagem = new Simec_Listagem();
		$listagem->setTamanhoPagina(50);
		$listagem->setFormFiltros('form-pesquisar-entrega');
		$listagem->turnOnPesquisator();
        $listagem->setCabecalho(array('Código Demanda', 'Tipo Ação','Descrição', 'Dt. Inclusão', 'Responsável', 'Área', 'Tipo','Relatório de Ocorrência', 'Estado', 'Data Estado' ));
        $listagem->addCallbackDeCampo(array('dmedsc', 'usunome'), 'alinhaParaEsquerda');

		$listagem->addAcao('edit', array('func' => 'editarEntrega', 'extra-params' => array('dmdid')));
        $listagem->addAcao('delete', 'apagarEntrega');

		$listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf']  )));
        $listagem->esconderColunas (array('usucpfinclusao','dmedtinclusao'));
        $listagem->setQuery($sql);
        $listagem->render();
    }
}