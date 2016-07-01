<?php
class CumprimentoObjetoDocumentacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_documentacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("codid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'codid' => null,
        'coid' => null,
        'codstatus' => null,
        'usucpf' => null,
        'coddatainclusao' => null
    );

    protected $arDados = array(
        'obraAtiva' => null,
        'obraVinculada' => null,
    );

    /**
    * Retorna a obra original
    * @param $obrid
    * @return mixed
    */
    private function pegaObraOriginal($obrid)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A')
            WHERE obr.obrid = {$obrid}
                AND obrstatus IN ('A')
            ";
        $obra = $this->pegaLinha($strSQL);
        
        if($obra) {
            return $obra;
        }

        $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obrid = {$obrid} AND obrstatus IN ('P')";
        $obra = $this->pegaLinha($strSQL);
        return $this->pegaObraOriginal($obra['obridvinculado']);
    }

    /**
     * Pega todas as obras vinculadas
     *
     * @param $obrid
     * @param array $vinculados
     * @return array
     */
    private function pegaObrasVinculadas($obrid, $vinculados = array())
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A')
            WHERE obr.obridvinculado = {$obrid}
                AND obr.obrstatus IN ('P');
        ";
        $vinculada = $this->pegaLinha($strSQL);
        
        if ($vinculada) {
            $vinculados[] = $vinculada;
            return $this->pegaObrasVinculadas($vinculada['obrid'], $vinculados);
        }

        return $vinculados;
    }

    private function verificaExistencia($obrid,$itcid)
    {
        $strSQL = "SELECT cod.codid FROM {$this->stNomeTabela} cod RIGHT JOIN obras2.cumprimento_objeto co ON (cod.coid=co.coid) WHERE co.obrid = $obrid;";
        $codid = $this->pegaUm($strSQL);
        if($codid) {
            $strSQL = "SELECT qstid FROM obras2.questao_cumprimento_obj_documentacao WHERE codid = $codid LIMIT 1";
            $qstid = $this->pegaUm($strSQL);
            $strSQL = "SELECT 1 FROM obras2.questao WHERE qstid = $qstid AND itcid = $itcid LIMIT 1";
            if($this->pegaUm($strSQL)){
                return true;
            }
        }
    }

    private function capturaDadosObra($obrid)
    {
        if(!$obrid) {
            return false;
        }

        $this->arDados['obraAtiva'] = $this->pegaObraOriginal($obrid);
        if(!$this->arDados['obraAtiva']) {
            return false;
        }
        $this->arDados['obraVinculada'] = $this->pegaObrasVinculadas($this->arDados['obraAtiva']['obrid']);
        return true;
    }

    public function montaQuestionario($obrid)
    {
        if(!$this->capturaDadosObra($obrid)){
            return false;
        }
        $questionarioCumprimentoObjeto = new QuestionarioCumprimentoObjeto();
        $questionarioCumprimentoObjeto->montaQuestionario($this->arDados['obraAtiva']);
        $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
        $questionarioCumprimentoObjetoConstrutora->montaQuestionario($this->arDados['obraAtiva'],false);
        $questionario = array(
            'questionario' => $questionarioCumprimentoObjeto,
            'questionarioConstrutora' => $questionarioCumprimentoObjetoConstrutora);

        #Verifica a existencia de obras vinculadas.
        $vinculadas = array();
        if($this->arDados['obraVinculada']) {
            foreach($this->arDados['obraVinculada'] as $obraVinculada) {
                $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
                $vinculadas[] = array(
                    'questionarioConstrutora' => $questionarioCumprimentoObjetoConstrutora->montaQuestionario($obraVinculada,false),
                    );
            }
        }
        $questionario['vinculadas'] = $vinculadas;
        return $questionario;
    }

    public function salvar($dados)
    {
        $codid = $dados['codid'];
        if(!$codid) {
            $this->popularDadosObjeto(
                array('coid' => $dados['coid'],
                    'codstatus' => 'A',
                    'usucpf' => $_SESSION['usucpf'],
                    'coddatainclusao' => 'NOW()'));
            $codid = $this->inserir();
        }
        
        $this->salvarQuestoes($codid,$dados['qcodresposta']);
        return true;
    }

    public function salvarQuestoes($codid, $questoes)
    {
        if(!$codid) {
            return false;
        }
        foreach($questoes as $qstid => $resposta) {
            $questionarioCumprimento = new QuestionarioCumprimentoObjeto();
            $arAtributos = array(
                'qstid' => $qstid,
                'qcodresposta' => ($_FILES[$qstid]['size'] > 0) ? 't' : $resposta,
                'qcodobs' => '',
                'qcodstatus' => 'A',
                'cumid' => $codid
            );
            $questionarioCumprimento->popularDadosObjeto($arAtributos);
            if(($qcodid = $questionarioCumprimento->verificaExistencia())) {
                $arAtributos = array('qcodid' => $qcodid);
                $questionarioCumprimento->popularDadosObjeto($arAtributos);
                $questionarioCumprimento->alterar();
            }else {
                $qcodid = $questionarioCumprimento->inserir();
            }
            $this->insereAnexo($qstid,$qcodid);
        }
    }

    public function insereAnexo($qstid, $qcodid)
    {
        if($_FILES[$qstid]['size'] > 0) {
            $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
            $questaoCumprimentoArquivo->novo($qstid,$qcodid);
        }
    }

    public function validaRespostasDocumentacao($obrid)
    {
        if(!$this->capturaDadosObra($obrid)){
            return false;
        }
        $questionarioCumprimentoObjeto = new QuestionarioCumprimentoObjeto();
        
        if ($questionarioCumprimentoObjeto->respostas($this->arDados['obraAtiva']) > 0) {
            return false;
        }
        
        $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
        if ($questionarioCumprimentoObjetoConstrutora->respostas($this->arDados['obraAtiva'],false) > 0) {
            return false;
        }
        
        if($this->arDados['obraVinculada']) {
            foreach($this->arDados['obraVinculada'] as $obraVinculada) {
                $questionarioCumprimentoObjetoConstrutora = new QuestionarioCumprimentoObjeto();
                if($questionarioCumprimentoObjetoConstrutora->respostas($obraVinculada,false) > 0) {
                    return false;
                }
            }
        }
        return true;
        
    }
}
