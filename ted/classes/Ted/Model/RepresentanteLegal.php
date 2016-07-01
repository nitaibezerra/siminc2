<?php

/**
 * Class Ted_Model_RepresentanteLegal
 */
class Ted_Model_RepresentanteLegal extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ted.representantelegal';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('rlid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rlid' => NULL,
        'ug' => NULL,
        'cpf' => NULL,
        'nome' => NULL,
        'email' => NULL,
        'status' => NULL,
        'funcao' => NULL,
        'substituto' => NULL
    );

    /**
     * @param $rlid
     * @return array|bool|void
     */
    public function get($rlid)
    {
        $strSQL = "
            SELECT
                rlid,
                uni.unicod,
                uni.unidsc,
                ung.ungdsc,
                ug as ungcod,
                cpf,
                nome,
                email,
                substituto
            FROM {$this->stNomeTabela} rpl
            JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
            LEFT JOIN public.unidade uni ON uni.unicod = ung.unicod
            WHERE rpl.rlid = {$rlid}
        ";
        $retorno = $this->pegaLinha($strSQL);
        return ($retorno) ? $retorno : false;
    }

    /**
     * Pega responsavel legal de uma unidade gestora
     * @param $ungcod
     * @return array|bool|null|void
     */
    public function pegaResponsavelUG($ungcod, $substituto = 'f')
    {
        $strSQL = "
            SELECT
                rlid,
				ug,
				cpf as usucpf,
				nome as usunome,
				email as usuemail,
				status as rpustatus,
				substituto
			FROM
				{$this->stNomeTabela}
			WHERE
				status = 'A' and substituto = '{$substituto}'
			".(is_array($ungcod) ? " AND ug IN ('".implode("','", $ungcod)."')" : " AND ug = '{$ungcod}'");
        //ver($strSQL, d);
        $retorno = $this->pegaLinha($strSQL);

        if ($retorno) {
            $retorno['usunome'] = utf8_encode($retorno['usunome']);
            return $retorno;
        } else {
            return false;
        }
    }

    /**
     * @param $ungcod
     * @return array|bool|void
     */
    public function recuperaResponsavelUG($ungcod)
    {
        $strSQL = "
            SELECT
                COALESCE(rl.cpf, 'Não informado') AS usucpf,
                COALESCE(rl.nome, 'Não informado') AS usunome,
                COALESCE(ungendereco, 'Não informado') AS endereco,
                COALESCE(ungbairro, 'Não informado') AS bairro,
                COALESCE(mun.mundescricao, 'Não informado') AS municipio,
                COALESCE(est.estdescricao, 'Não informado') AS estado,
                COALESCE(ungcep, 'Não informado') AS endcep,
                COALESCE(su.usufoneddd||'-'||su.usufonenum, 'Não informado') AS fone,
                COALESCE( etu.entnumrg, COALESCE( et.entnumrg, 'Não informado') ) AS numeroidentidade,
	            COALESCE( etu.entorgaoexpedidor, COALESCE( et.entorgaoexpedidor, 'Não informado') ) AS entorgaoexpedidor,
                COALESCE(su.usufuncao, 'Não informado') AS usufuncao,
                COALESCE(rl.email, 'Não informado') AS usuemail
            FROM public.unidadegestora ung
            LEFT JOIN ted.representantelegal rl ON (rl.ug = ung.ungcod)
            LEFT JOIN territorios.municipio mun ON mun.muncod = ung.muncod
            LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
            LEFT JOIN seguranca.usuario su ON (su.usucpf = rl.cpf)
            LEFT JOIN entidade.entidade et ON et.entid = su.entid
            LEFT JOIN entidade.entidade etu ON etu.entnumcpfcnpj = su.usucpf
            WHERE ung.ungstatus = 'A' AND ung.ungcod = '{$ungcod}' AND rl.substituto = 'f'
        ";

        //ver($strSQL);
    	return $this->pegaLinha($strSQL);
    }

    /**
     * Area Tecnica Responsavel Proponente
     * @param $ted
     * @return array|bool|void
     */
    public function areaTecnicaResponsavel($ted)
    {
        $strSQL = "
            select
                u.usunome, wh.usucpf
            from workflow.historicodocumento wh
            inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
            where
                wh.docid = (select docid from ted.termocompromisso where tcpid = {$ted})
                and wh.aedid = 1595
        ";

        return $this->pegaLinha($strSQL);
    }

    /**
     * Coordenação responsavel Concedente
     * @param $ted
     * @return array|bool|void
     */
    public function coordenacaoResponsavel($ted)
    {
        $strSQL = "
            select
                u.usunome, wh.usucpf
            from workflow.historicodocumento wh
            inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
            where
                wh.docid = (select docid from ted.termocompromisso where tcpid = {$ted})
                and wh.aedid in (1605, 1610)
            order by wh.hstid desc limit 1
        ";

        return $this->pegaLinha($strSQL);
    }

    /**
     * @param array $data
     * @param null $ug
     */
    public function save(array $data, $context = null)
    {
        if (!$this->isValid($data)) {
            return false;
        }

        if (!empty($_POST['rlid']) && is_numeric($_POST['rlid'])) {
            $sql = "
                update {$this->stNomeTabela} set
                        cpf = '{$this->cleanUPCPF($data['cpf'])}',
                        nome = '{$data['nome']}',
                        email = '{$data['email']}',
                        substituto = '{$data['substituto']}'
                 where rlid = '{$data['rlid']}';
            ";
        } else {
            $sql = "
                insert into {$this->stNomeTabela}
                    (cpf, nome, email, ug, substituto, status)
                values (
                    '{$this->cleanUPCPF($data['cpf'])}',
                    '{$data['nome']}',
                    '{$data['email']}',
                    '{$data['ungcod']}',
                    '{$data['substituto']}',
                    'A'
                );
            ";
        }

        //ver($sql, d);
        $this->executar($sql);
        return ($this->commit()) ? true : false;
    }

    /**
     * faz validação de preenchimento dos dados do representate legal substituto
     * @param array $data
     * @return bool
     */
    public function isValid(array $data)
    {
        $arrayKeys = array('cpf', 'nome', 'email');
        foreach ($data as $k => $value) {
            foreach ($arrayKeys as $i => $field) {
                if (($arrayKeys[$i] == $k)) {
                    if (empty($value)) return false;
                }
            }
        }

        return true;
    }

    /**
     * @param $cpf
     * @return bool
     */
    public function findByCPF($cpf)
    {
        $strSQL = "select cpf from {$this->stNomeTabela} where cpf = '{$this->cleanUPCPF($cpf)}'";
        $result = $this->pegaUm($strSQL);
        return ($result) ? true : false;
    }

    /**
     * @param $cpf
     * @return mixed
     */
    private function cleanUPCPF($cpf)
    {
        if (strlen($cpf)) {
            $cpfClean = str_replace(array('.', '-'), '', $cpf);
            return $cpfClean;
        }
    }

    /**
     * @param $tcpid
     * @param $context
     * @return array|bool|null|void
     */
    public function getSubstituto($context)
    {
        $ator = ($context == 'proponente') ? 'ungcodproponente' : 'ungcodconcedente';

        $ted = new Ted_Model_TermoExecucaoDescentralizada();
        $dadosTed = $ted->pegaTermoCompleto();

        $strSQL = "select rlid, cpf, nome, email from {$this->stNomeTabela} where ug = '{$dadosTed[$ator]}' and substituto = 't'";
        //ver($strSQL, d);
        $linha = $this->pegaLinha($strSQL);
        return ($linha) ? $linha : null;
    }
}