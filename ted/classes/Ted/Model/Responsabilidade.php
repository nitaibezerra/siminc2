<?php

/**
 * Class Ted_Model_Responsabilidade
 */
class Ted_Model_Responsabilidade extends Modelo
{
    /**
     * @var
     */
    protected $_where = array();

    /**
     * @var array
     */
    protected $_perfis = array();

    /**
     * @var array
     */
    protected $_roles = array();

    /**
     * @var array
     */
    protected $_ungcod = array();

    /**
     * @var array
     */
    protected $_unicod = array();

    /**
     * @var array
     */
    protected $_cooid = array();

    /**
     * @var
     */
    protected $_usucpf;

    /**
     * identificador das permissões
     */
    const SUPER_USUARIO = 1233;
    const CONSULTA = 1274;
    const GESTOR_ORCAMENTARIO_PROPONENTE = 1262;
    const REPRESENTANTE_LEGAL_PROPONENTE = 1263;
    const GABINETE_SECRETARIA_AUTARQUIA = 1264;
    const COORDENADOR_SECRETARIA_AUTARQUIA = 1265;
    const DIRETORIA_SECRETARIA_AUTARQUIA = 1266;
    const REPRESENTANTE_LEGAL_CONCEDENTE = 1267;
    const GESTOR_ORCAMENTARIO_CONCEDENTE = 1268;
    const AREA_TECNICA_FNDE = 1269;
    const DIRETORIA_FNDE = 1270;
    const UO_EQUIPE_TECNICA = 1271;
    const UG_REPASSADORA = 1273;

    /**
     * @var array
     */
    protected $_responsabilidades = array(
        'proponente' => array(
            self::GESTOR_ORCAMENTARIO_PROPONENTE,
            self::REPRESENTANTE_LEGAL_PROPONENTE
        ),
        'concedente' => array(
            self::GABINETE_SECRETARIA_AUTARQUIA,
            self::COORDENADOR_SECRETARIA_AUTARQUIA,
            self::DIRETORIA_SECRETARIA_AUTARQUIA,
            self::REPRESENTANTE_LEGAL_CONCEDENTE,
            self::GESTOR_ORCAMENTARIO_CONCEDENTE,
            self::AREA_TECNICA_FNDE,
            self::DIRETORIA_FNDE,
            self::UG_REPASSADORA
        ),
        'commons' => array(
            self::UO_EQUIPE_TECNICA
        )
    );

    const FNDE = '153173';
    const CAPES = '154003';
    const INEP = '153978';

    const SECADI = '150028';
    const SETEC = '150016';
    const SEB = '150019';

    /**
     * constructor
     */
    public function __construct($usucpf = null)
    {
        $this->_usucpf = ($usucpf) ? $usucpf : $_SESSION['usucpf'];
        if (!$this->_usucpf) {
            throw new Exception('usucpf is not null');
        }

        $this->_populateRoles();
        $this->_associateRules();
        return this;
    }

    /**
     * Matriz com responsabilidades do usuario logado
     * @return array|null|void
     */
    protected function _populateRoles()
    {
        $strSQL = "
            SELECT
                ur.pflcod, ur.unicod, ur.prsano, ur.ungcod, ur.cooid, ur.dircod
            FROM ted.usuarioresponsabilidade ur
            INNER JOIN
                seguranca.perfil pf ON pf.pflcod = ur.pflcod AND pflstatus = 'A'
            WHERE
                usucpf = '%s' AND
                rpustatus = 'A'
        ";

        $stmt = sprintf($strSQL, $this->_usucpf);
        $this->_roles = $this->carregar($stmt);
    }

    /**
     * @return array|null
     */
    public function getRoles()
    {
        return (count($this->_roles)) ? $this->_roles : null;
    }

    /**
     * Seta as permissões de acordo com as responsabilidades
     * vinculadas ao perfil
     * @return void(0)
     */
    protected function _associateRules()
    {
        if (is_array($this->_roles) && count($this->_roles)) {
            foreach ($this->_roles as $role) {

                if (!empty($role['cooid'])) {
                    array_push($this->_cooid, $role['cooid']);
                }

                if (!empty($role['pflcod'])) {
                    array_push($this->_perfis, $role['pflcod']);
                }

                if (!in_array($role['ungcod'], $this->_ungcod) && strlen(trim($role['ungcod'])) == 6) {
                    array_push($this->_ungcod, $role['ungcod']);
                }

                if (!in_array($role['unicod'], $this->_unicod) && strlen(trim($role['unicod'])) == 5) {
                    array_push($this->_unicod, $role['unicod']);
                }
            }
        }
    }

   
    /**
     * Filtro de unidades associadas a termos de compromissos
     * See: Ted_Model_UnidadeGestora::pegaListaProponente()
     * @return bool|string
     */
    public function filtroUG()
    {
        if ($this->getUngCod()) {

            $arrUngcod = $this->getUngCod();
            $params = "'".implode("','", $arrUngcod)."'";

            $strSQL = "
                select Distinct ungcodproponente, ungcodconcedente from ted.termocompromisso
                where (ungcodproponente in ({$params}) OR ungcodconcedente in ({$params}))
                and ungcodproponente is not null and ungcodconcedente is not null
                and tcpstatus = 'A'
            ";

            $options = array();
            if ($results = $this->carregar($strSQL)) {
                foreach ($results as $ungcod) {
                    if (!in_array($ungcod['ungcodproponente'], $options)) {
                        array_push($options, $ungcod['ungcodproponente']);
                    }

                    if (!in_array($ungcod['ungcodconcedente'], $options)) {
                        array_push($options, $ungcod['ungcodconcedente']);
                    }
                }
            }

            if (count($options)) {
                return "'".implode("','", $options)."'";
            }

            return false;
        }
    }

    /**
     * Retorna a clausula condicional
     * de acordo com as responsabilidades setadas
     * @return array
     */
    public function getClausleWhere()
    {
        if (possui_perfil(array(PERFIL_UG_REPASSADORA, PERFIL_CGSO))) {
            return false;
        }

        $where = array();

        $secretariasFNDE = array(
            self::SECADI,
            self::SETEC,
            self::SEB
        );

        if ($this->getUngCod()) {

            $arrUngcod = $this->getUngCod();

            foreach ($arrUngcod as $ungcod) {
                if (in_array($ungcod, $secretariasFNDE)) {
                    array_push($arrUngcod, self::FNDE);
                    break;
                }
            }

            $params = "'".implode("','", $arrUngcod)."'";
            $where[] = " (unc.ungcod in ({$params}) OR unp.ungcod in ({$params})) ";
        }
       
       
        if ($this->getCoordenacao()) {
            $params = "'".implode("','", $this->getCoordenacao())."'";
            $where[] = " (cdn.cooid in ({$params})) ";
        }

        $where =  implode(' OR ', $where);
        
         /* Apenas para perfil de Secretário, para não haver aprovação incorreta */
        if (possui_perfil(array(PERFIL_SECRETARIO)) && !isset($_REQUEST['ted'])) {
            $params = "'".implode("','", $arrUngcod)."'";
            $where .= " AND (tcp.tcpid IN ((SELECT tcpid FROM ted.coordenacao_responsavel WHERE ungcod IN ({$params}) )) 
                             OR tcp.ungcodpoliticafnde IN ({$params}) 
                             OR tcp.ungcodconcedente IN ({$params}) 
                             OR tcp.ungcodemitente IN ({$params}) )";
        }
        #var_dump($_REQUEST); die;
        
        return ($where!='') ? $where : false;

    }

    /**
     * @return array|bool
     */
    public function getUngCod()
    {
        if (count($this->_ungcod)) {
            return $this->_ungcod;
        }

        return false;
    }

    /**
     * @return array
     */
    public function getPerfis()
    {
        if (count($this->_perfis)) {
            return $this->_perfis;
        }

        return false;
    }

    /**
     * @return array
     */
    public function getUnidades()
    {
        if (count($this->_unicod)) {
            return $this->_unicod;
        }

        return false;
    }

    /**
     * @return array
     */
    public function getCoordenacao()
    {
        if (count($this->_cooid)) {
            return $this->_cooid;
        }

        return false;
    }

    /**
     * @return array|bool
     */
    public function getDivisaoPerfis()
    {
        if (count($this->_responsabilidades)) {
            return $this->_responsabilidades;
        }

        return false;
    }
}