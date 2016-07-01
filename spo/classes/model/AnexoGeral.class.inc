<?php
/**
 * Description of Snapshot
 *
 * @author LindalbertoFilho
 */
class Spo_Model_AnexoGeral extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spo.anexogeral";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("arqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'arqid' => NULL,
        'angdsc' => NULL,
        'pdcid' => NULL,
        'arpdtinclusao' => NULL,
        'arpstatus' => NULL,
        'arpdsc' => NULL,
        'arptipo' => NULL,
        'angtipoanexo' => NULL
    );

    private $queryAnexo = <<<DML
        SELECT
            anx.arqid,
            anx.angdsc,
            arq.arqtamanho,
            anx.arptipo,
            to_char(arq.arqdata,'DD/MM/YYYY')||' '||arq.arqhora as arqdata,
            usu.usunome
        FROM spo.anexogeral anx
        INNER JOIN public.arquivo arq on anx.arqid = arq.arqid
        INNER JOIN seguranca.usuario usu on arq.usucpf = usu.usucpf
        WHERE anx.arptipo = '%s' AND anx.angtipoanexo = '%s' AND anx.exercicio = '%s'
DML;

    private $queryAnexoPorId = <<<DML
        SELECT
            anx.arqid,
            anx.angdsc,
            arq.arqnome,
            arq.arqextensao,
            anx.arptipo
        FROM spo.anexogeral anx
        INNER JOIN public.arquivo arq on anx.arqid = arq.arqid
        WHERE anx.arqid = %s
DML;

    private $queryAlteraAnexo = <<<DML
        UPDATE spo.anexogeral
            SET angdsc = '%s', arptipo = '%s'
        WHERE arqid = '%s'
DML;

    private $queryDownload = <<<DML
        SELECT 
            arqid,
            arqnome,
            arqdescricao,
            arqextensao,
            arqtipo,
            arqtamanho,
            arqdata,
            arqstatus,
            usucpf,
            sisid
        FROM public.arquivo
        WHERE arqid = %s
DML;

    private $queryPermissaoAcesso = <<<DML
        SELECT 1
        FROM seguranca.perfilusuario
        WHERE usucpf = '%s'
            AND pflcod IN (
                SELECT pflcod
                FROM seguranca.perfil
                WHERE (pfldsc ILIKE '%%cgp gestão%%' OR pfldsc ILIKE '%%cgo%%')
                    AND sisid = %s
                )
DML;
    
    /**
     * Popular variáveis: arptipo, angtipoanexo;
     * @return array('arqid','angdsc','arqtamanho','arptipo','arqdata','usunome')
     */
    public function recuperaAnexoGeral()
    {
        $query = sprintf($this->queryAnexo,$this->arAtributos['arptipo'],$this->arAtributos['angtipoanexo'],$_SESSION['exercicio']);
        return $this->carregar($query);
    }

    /**
     * Popular variável arqid.
     * @return array('arqid', 'angdsc', 'arqnome', 'arqextensao', 'arptipo')
     */
    public function buscaAnexo()
    {
        $query = sprintf($this->queryAnexoPorId,$this->arAtributos['arqid']);
        return $this->pegaLinha($query);
    }

    /**
     * Popular variáveis: angdsc, arptipo, arqid;
     * @return boolean
     */
    public function alteraAnexo()
    {
        $query = sprintf($this->queryAlteraAnexo,  $this->arAtributos['angdsc'], $this->arAtributos['arptipo'], $this->arAtributos['arqid']);
        $this->executar($query);
        return $this->commit();
    }

    public function buscaDadosArquivo()
    {
        $query = sprintf($this->queryDownload, $this->arAtributos['arqid']);
		return $this->pegaLinha($query);
    }

    public function permissaoAcesso()
    {
        if($this->testa_superuser()){
            return true;
        }
        $query = sprintf($this->queryPermissaoAcesso,$_SESSION['usucpf'],$_SESSION['sisid']);
        if($this->pegaUm($query)){
            return true;
        }
        return false;
    }
}
