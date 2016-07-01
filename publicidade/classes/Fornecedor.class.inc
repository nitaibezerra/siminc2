<?php

/**
 * Modelo de Fornecedor
 * @author Alysson Rafael
 */
class Fornecedor extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'publicidade.fornecedor';

    /**
     * Chave primária
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('forid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'fornome' => null,
        'forlog' => null,
        'fortel' => null,
        'forcnpj' => null,
        'forfax' => null,
        'forramal' => null,
        'forst' => null,
        'forobs' => null,
        'forcep' => null,
        'forcomp' => null,
        'fornum' => null,
        'forbai' => null,
        'foremail' => null,
        'estuf' => null,
        'muncod' => null,
        'fordata' => null,
        'usucpf' => null,
        'foragencia' => null,
        'forstatus' => null
    );

    /**
     * Gerar a combo de situação
     * @name monta_combo_situacao
     * @author Alysson Rafael
     * @access public
     * @return void
     */
    public function monta_combo_situacao() {

        $situacoes = array(array('codigo' => 'A', 'descricao' => 'Em Atividade'), array('codigo' => 'E', 'descricao' => 'Extinta'), array('codigo' => 'P', 'descricao' => 'Paralisada'));

        $this->monta_combo('forst', $situacoes, 'S', 'Selecione...', '', '', '', '', 'N', 'forst', '', $forst);
    }

    /**
     * Listagem de fornecedores
     * @name listar_por_filtro
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return void
     */
    public function listar_por_filtro($post) {

        $where = " WHERE fornecedor.forstatus='A' ";
        if (!empty($post['fornome'])) {
            $post['fornome'] = tirar_acentos(pg_escape_string($post['fornome']));
            $where .= " AND(TRANSLATE(fornecedor.fornome, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['fornome']}%' OR fornecedor.fornome ILIKE '%{$post['fornome']}%') ";
            //$where .= " AND fornecedor.fornome ILIKE '%{$post['fornome']}%' OR fornecedor.fornome ILIKE '%{$post['fornome']}%') ";
        }
        if (!empty($post['forcnpj'])) {
            $post['forcnpj'] = str_replace(array('/', '-', '.'), '', $post['forcnpj']);
            $post['forcnpj'] = pg_escape_string($post['forcnpj']);
            $where .= " AND fornecedor.forcnpj = '{$post['forcnpj']}' ";
        }
        if (!empty($post['estuf'])) {
            $post['estuf'] = pg_escape_string($post['estuf']);
            $where .= " AND fornecedor.estuf = '{$post['estuf']}' ";
        }
        if (isset($post['foragencia'])) {
            $post['foragencia'] = pg_escape_string($post['foragencia']);
            $where .= " AND fornecedor.foragencia = {$post['foragencia']} ";
        }


        $sql = "SELECT 
				  '<center>
                  <a style=\"cursor:pointer;\" onclick=\"detalhar(\''||fornecedor.forid||'\');\">
                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                  </a>
                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.contrato contrato WHERE contrato.forid=fornecedor.forid) = 0 AND (SELECT COUNT(*) FROM publicidade.itenspad item WHERE item.forid=fornecedor.forid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||fornecedor.forid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do!\');' END || '\">
                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                  </a>
                  </center>' as acao,
                  fornecedor.fornome AS fornome,
                  SUBSTR(CAST(fornecedor.forcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 13 , 2) AS cnpj,
                  fornecedor.estuf AS estuf,
                  municipio.mundescricao AS mundescricao,
                  fornecedor.foremail AS foremail,
                  
                  CASE WHEN
                  	fornecedor.fortel <> ''
                  THEN
                  	'('||SUBSTR(CAST(fornecedor.fortel AS text),1,2)||') '||SUBSTR(CAST(fornecedor.fortel AS text),3)
                  ELSE
                  	''
                  END AS fortel,
                  
                  CASE WHEN
                  	fornecedor.foragencia = 0
                  THEN
                  	'N�o'
                  ELSE
                  	'Sim'
                  END
                  
                  FROM $this->stNomeTabela fornecedor 
                  LEFT JOIN territorios.municipio municipio ON fornecedor.muncod=municipio.muncod
                  {$where} 
                  ORDER BY fornecedor.fornome ";

        $cabecalho = array('A��o', 'Raz�o Social', 'CNPJ', 'UF', 'Munic�pio', 'E-mail', 'Telefone', 'Ag�ncia');
        $this->monta_lista($sql, $cabecalho, 10, 50, false, "center", 'S', '', '', array('center', 'left', 'center', 'center', 'center', 'left', 'center', 'center'));
    }

    /**
     * Verifica se um fornecedor já está cadastrado
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - Dados do formulário
     * @access public
     * @return bool
     */
    public function verifica_unico($post) {
        if (!empty($post['forcnpj'])) {
            $post['forcnpj'] = str_replace(array('-', '.', '/'), '', $post['forcnpj']);
            $post['forcnpj'] = pg_escape_string($post['forcnpj']);

            $sql = "SELECT fornecedor.forid FROM $this->stNomeTabela fornecedor ";
            $sql .= "WHERE fornecedor.forcnpj='{$post['forcnpj']}' AND fornecedor.forstatus='A' ";

            //caso seja atualização, exclui o registro atual da verificação
            if (!empty($post['forid'])) {
                $post['forid'] = pg_escape_string($post['forid']);
                $sql .= "AND fornecedor.forid <> '{$post['forid']}' ";
            }

            $forid = $this->pegaUm($sql);

            if ($forid >= 1) {
                return true;
            } else {
                return false;
            }
        }
    }

    /**
     * Cadastrar/editar 
     * @name salvar
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
    public function salvar($post) {
        extract($post);

        $forcnpj = str_replace(array('-', '.', '/'), '', $forcnpj);
        $forcnpj = pg_escape_string($forcnpj);
        $fornome = pg_escape_string($fornome);
        $foremail = pg_escape_string($foremail);

        $fortel = str_replace('-', '', $fortel);
        $fortel = pg_escape_string($dddcomercial) . pg_escape_string($fortel);
        $forramal = pg_escape_string($forramal);
        $forfax = str_replace('-', '', $forfax);
        $forfax = pg_escape_string($dddfax) . pg_escape_string($forfax);

        if (!empty($forst)) {
            $forst = pg_escape_string($forst);
        } else {
            $forst = 'A';
        }

        $forobs = pg_escape_string($forobs);

        if (!empty($forcep)) {
            $forcep = str_replace(array('-', '.'), '', $forcep);
            $forcep = pg_escape_string($forcep);
        } else {
            $forcep = 'null';
        }

        $forlog = pg_escape_string($forlog);
        $forcomp = pg_escape_string($forcomp);
        $fornum = pg_escape_string($fornum);
        $forbai = pg_escape_string($forbai);
        $muncod = pg_escape_string($muncod);
        $estuf = pg_escape_string($estuf);
        $foragencia = pg_escape_string($foragencia);

        $operacao = '';

        //caso o id esteja vazio, a operação será de insert
        //caso não esteja vazio, a operação será de update
        if (empty($forid)) {
            $sql = "INSERT INTO $this->stNomeTabela(fornome,forlog,fortel,forcnpj,forfax,forramal,";
            $sql .= "forst,forobs,forcep,forcomp,fornum,forbai,foremail,fordata,usucpf,foragencia";
            if (!empty($estuf)) {
                $sql .= ",estuf";
            }
            if (!empty($muncod)) {
                $sql .= ",muncod";
            }
            $sql .= ")VALUES('{$fornome}','{$forlog}','{$fortel}','{$forcnpj}','{$forfax}','{$forramal}',";
            $sql .= "'{$forst}','{$forobs}',{$forcep},'{$forcomp}','{$fornum}','{$forbai}','{$foremail}',";
            $sql .= "'" . date('Y-m-d H:i:s') . "','" . $_SESSION['usucpf'] . "',{$foragencia}";

            //verifica se foi selecionado uf e cidade para incluir no insert
            if (!empty($estuf)) {
                $sql .= ",'{$estuf}'";
            }
            if (!empty($muncod)) {
                $sql .= ",'{$muncod}'";
            }
            $sql .= ")";
        } else if (!empty($forid)) {
            $forid = pg_escape_string($forid);
            $sql = "UPDATE $this->stNomeTabela SET fornome='{$fornome}',forlog='{$forlog}',fortel='{$fortel}',";
            $sql .= "forcnpj='{$forcnpj}',forfax='{$forfax}',forramal='{$forramal}',forst='{$forst}',";
            $sql .= "forobs='{$forobs}',forcep={$forcep},forcomp='{$forcomp}',fornum='{$fornum}',";
            $sql .= "forbai='{$forbai}',foremail='{$foremail}',fordata='" . date('Y-m-d H:i:s') . "',";
            $sql .= "usucpf='" . $_SESSION['usucpf'] . "',foragencia={$foragencia} ";

            if (empty($estuf)) {
                $estuf = 'NULL';
                $sql .= ",estuf={$estuf} ";
            } else {
                $sql .= ",estuf='{$estuf}' ";
            }

            if (empty($muncod)) {
                $muncod = 'NULL';
                $sql .= ",muncod={$muncod} ";
            } else {
                $sql .= ",muncod='{$muncod}' ";
            }


            $sql .= "WHERE forid='{$forid}' ";
        }

        $sql .= " RETURNING forid ";

        $forid = $this->pegaUm($sql);

        if (!empty($forid)) {
            $this->commit();
            return array(0 => true, 1 => $forid, 2 => 'listaFornecedor');
        } else {
            return array(0 => false, 1 => $forid, 2 => 'listaFornecedor');
        }
    }

    /**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $forid - Identificador do registro
     * @return array
     */
    public function carrega_registro_por_id($forid) {
        if (!empty($forid)) {
            $forid = pg_escape_string($forid);
            $sql = "SELECT fornecedor.forid,
						   fornecedor.fornome,
						   fornecedor.forlog,
						   fornecedor.fortel,
						   SUBSTR(CAST(fornecedor.forcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcnpj AS text) , 13 , 2) AS forcnpj,
						   fornecedor.forfax,
						   fornecedor.forramal,
						   fornecedor.forst,
						   fornecedor.forobs,
						   fornecedor.forcep,
						   fornecedor.forcomp,
						   fornecedor.fornum,
						   fornecedor.forbai,
						   fornecedor.foremail,
						   fornecedor.estuf,
						   fornecedor.muncod,
						   fornecedor.foragencia
						   FROM $this->stNomeTabela fornecedor WHERE fornecedor.forid='{$forid}' ";
            return $this->pegaLinha($sql);
        }
    }

    /**
     * Inativa um registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $forid - Id do registro 
     * @return void
     */
    public function inativar($forid) {
        if (!empty($forid)) {
            $forid = pg_escape_string($forid);

            $sql = "UPDATE $this->stNomeTabela SET forstatus='I' WHERE forid='{$forid}' ";
            $this->executar($sql);
            $this->commit();
        }
    }

    /**
     * Gerar a combo de fornecedor
     * @name monta_combo_fornecedor
     * @author Alysson Rafael
     * @access public
     * @param int $forid          - Qual option receberá o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @param int    $agencia     - Caso seja para filtrar fornecedor do tipo agência
     * @param string $origem      - De onde foi chamada a função
     * @return void
     */
    public function monta_combo_fornecedor($forid = '', $obrigatorio = 'N', $agencia = '', $origem = '') {

        $sql = "SELECT DISTINCT fornec.forid AS codigo,fornec.fornome AS descricao FROM $this->stNomeTabela fornec ";
        //caso a chamada venha da tela de cadastro de pad
        //busca apenas as agências que têm contrato vinculado
        if ($origem == 'cadPad') {
            $sql .= "JOIN publicidade.contrato cont ON fornec.forid=cont.forid ";
        }

        $sql .= "WHERE fornec.forstatus='A' ";

        # caso a chamada venha da tela de cadastro de pad
        # busca apenas as agências que têm contrato vinculado
        if ($origem == 'cadPad') {
            $data = date('Y-m-d');
            $sql .= "AND (cont.cttvigenciainicio <= '{$data}' AND cont.cttvigenciatermino >= '{$data}') ";
        }

        if ($agencia == 1) {
            $sql .= "AND fornec.foragencia='{$agencia}' ";
        }
        
        if ($origem == 'cadPad') {
              $sql .=  "	union all
	SELECT DISTINCT fornec.forid AS codigo,fornec.fornome AS descricao 
	FROM publicidade.fornecedor fornec 
	JOIN publicidade.contrato cont ON fornec.forid=cont.forid 
	JOIN publicidade.contratoaditivo conta ON cont.cttid=conta.cttid 
	WHERE fornec.forstatus='A'";
        }
         if ($agencia == 1) {
            $sql .= "AND fornec.foragencia='{$agencia}' ";
        }
         if ($origem == 'cadPad') {
            $data = date('Y-m-d');
            $sql .= "AND (conta.ctavigenciainicio <= '{$data}' AND conta.ctavigenciatermino >= '{$data}') ";
        }

        $resultado = $this->carregar($sql);

        if (!is_array($resultado) || count($resultado) < 1) {
            $resultado = array();
        }


        if ($origem == 'cadPad') {
            $this->monta_combo('forid', $resultado, 'S', 'Selecione...', 'carregarContrato(this.value,\'successCarregarContrato\',\'failureCarregarContrato\',\'createCarregarContrato\',\'completeCarregarContrato\');', '', '', '', $obrigatorio, 'forid', '', $forid);
        } else if ($origem == 'cadItemPad') {
            //$this->monta_combo('forid',$resultado,'S','Selecione...','verificarHonorarioFornec(this.value);','','','',$obrigatorio,'forid','',$forid);
            if ($agencia != 1) {
                $this->monta_combo('forid_fornecedor', $resultado, 'S', 'Selecione...', '', '', '', '', $obrigatorio, 'forid', '', $forid);
            } else {
                $this->monta_combo('forid', $resultado, 'S', 'Selecione...', '', '', '', '', $obrigatorio, 'forid', '', $forid);
            }
        } else {
            if ($agencia != 1) {
                $this->monta_combo('forid_fornecedor', $resultado, 'S', 'Selecione...', '', '', '', '', $obrigatorio, 'forid', '', $forid);
            } else {
                $this->monta_combo('forid', $resultado, 'S', 'Selecione...', '', '', '', '', $obrigatorio, 'forid', '', $forid);
            }
        }
    }

    /**
     * Gerar a combo múltipla de agências
     * @name monta_combo_fornecedor_multiplo
     * @author Alysson Rafael
     * @access public
     * @return void
     */
    function monta_combo_fornecedor_multiplo() {

        $sql = "SELECT DISTINCT fornec.forid AS codigo,fornec.fornome AS descricao 
				FROM $this->stNomeTabela fornec 
				JOIN publicidade.contrato cont ON fornec.forid=cont.forid 
				WHERE fornec.forst='A' 
				AND fornec.foragencia='1' 
				ORDER BY fornec.fornome ";

        combo_popup('agencias', $sql, 'Selecione a(s) Ag�ncias(s)', '360x460', 0, array(), '', 'S');
    }

    /**
     * Pega as agências para o relatório
     * @name pegar_agencias_prestacao_contas
     * @author Alysson Rafael
     * @access public
     * @param array $get - Array com os filtros
     * @return array
     */
    public function pegar_agencias_prestacao_contas($get) {
        if (!empty($get['dtinicial']) && !empty($get['dtfinal'])) {
            $get['dtinicial'] = pg_escape_string(formata_data_sql($get['dtinicial']));
            $get['dtfinal'] = pg_escape_string(formata_data_sql($get['dtfinal']));
            $get['agencias'] = pg_escape_string($get['agencias']);

            $sql = "SELECT fornec.fornome AS fornome,
						   SUBSTR(cont.cttnumcontrato,1,3)||'/'||SUBSTR(cont.cttnumcontrato,4,4) AS cttnumcontrato
						   FROM $this->stNomeTabela fornec
						   JOIN publicidade.contrato cont ON fornec.forid=cont.forid
						   JOIN publicidade.pad pad ON cont.cttid=pad.cttid
						   JOIN publicidade.itenspad item ON pad.padid=item.padid
						   JOIN publicidade.pagamentoitempad pgto ON item.ipaid=pgto.ipaid
						   WHERE(pgto.pipdatapagamento >= '{$get['dtinicial']}' AND pgto.pipdatapagamento <= '{$get['dtfinal']}' AND pgto.pipstatus = 'A') 
						   AND fornec.forstatus='A'
						   AND fornec.foragencia='1'
						   AND fornec.forid IN ({$get['agencias']})
						   AND cont.cttstatus='A'
						   AND pad.padstatus='A' 
						   AND item.ipastatus='P'
						   GROUP BY fornec.fornome,cont.cttnumcontrato
						   ORDER BY fornec.fornome ";

            return $this->carregar($sql);
        }
    }

}
