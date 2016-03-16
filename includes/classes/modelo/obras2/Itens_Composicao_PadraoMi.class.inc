<?php
    
class Itens_Composicao_PadraoMi extends Modelo{
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.itenscomposicaopadraomi"; 

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'icmid' => null,  
                                        'cpmid' => null,  
                                        'itcid' => null,  
                                        'icmordem' => null,  
                                        'icmdiasinicio' => null,  
                                        'icmdiasduracao' => null,  
                                        'icmvalor' => null,  
                                        'icmpercentual' => null,  
                                        'icmstatus' => null, 
                                        'umdid'  => null,
                                        'icmquantidade' => null,
                                        'relativoEdificacao' => null,
                                        'icmvalortotal' => null
                                      );

    public function pegaTudoPorCpm ($cpmid, $relativoEdificacao = null) {
        $where = (!$relativoEdificacao) ? '' : 'AND ' . $relativoEdificacao;
        $sql = "
             SELECT
             icm.icmdiasinicio,
             icm.icmdiasduracao,
             icm.icmpercentual,
             icm.icmdiasinicio,
             icm.icmordem,
             icm.icmvalor,
             icm.umdid,
             icm.icmquantidade,
             icm.itcid,
             icm.icmid,
             icm.icmstatus,
             icm.icmvalortotal,
             icm.relativoEdificacao
             FROM obras2.itenscomposicaopadraomi icm
             WHERE
                icm.cpmid = {$cpmid}
                AND icm.icmstatus = 'A'
                {$where}
             ORDER BY icm.icmordem
            ";
        $dados = $this->carregar( $sql );
        return $dados;
    }

    public function pegaPorCpm($cpmid, $relativoEdificacao = null){
        $sql = "
             SELECT 
             icm.icmdiasinicio,
             icm.icmdiasduracao,
             icm.icmpercentual,
             icm.icmdiasinicio,
             icm.icmordem,
             icm.icmvalor,
             icm.umdid,
             icm.icmquantidade,
             icm.itcid,
             icm.icmid,
             icm.icmstatus,
             icm.icmvalortotal
             FROM obras2.itenscomposicaopadraomi icm
             WHERE 
                icm.cpmid = {$cpmid} 
                AND icm.icmstatus = 'A'  
                AND (icm.relativoEdificacao = '{$relativoEdificacao}' " . (($relativoEdificacao!='F')?" OR icm.relativoEdificacao = 'D' ":"") . ")
             ORDER BY icm.icmordem ASC
            ";
        
        $dados = $this->carregar( $sql );
        return $dados;
    }
    
    public function salvarFormulario()
    {
        // Guarda na tabela Cronogramapadraomi
        $cronogramaDados['cpmid'] = ($_POST['cpmid']) ? $_POST['cpmid'] : null;
        $cronogramaDados['estuf'] = $_POST['estuf'];
        $cronogramaDados['tmiid'] = $_POST['tmiid'];
        $cronogramaDados['cpmvalor'] = number_format(str_replace('.', '', $_REQUEST['cpmvalor']),'2','.','');
        $cronogramaDados['cpmstatus'] = 'A';
        $cronogramaDados['tpoid'] = $_POST['tpoid'];

        $cronograma = new Cronograma_PadraoMi();
        $cronograma->popularDadosObjeto($cronogramaDados);
        $cpmid = $cronograma->salvar();
        $cronograma->commit();
        
        if($_POST['itcid']){
            foreach($_POST['itcid'] as $key => $itcid){
                $this->clearDados();
                
                if (!$itcid)
                    continue;
                
                $countTmp = ($key+1);
                $arrDados['itcid'] = $itcid;
                $arrDados['icmordem'] = $countTmp;
                $arrDados['icmdiasinicio'] = ($_POST['icmdiasinicio'][$key])?$_POST['icmdiasinicio'][$key]:null;
                $arrDados['icmdiasduracao'] = ($_POST['icmdiasduracao'][$key])?$_POST['icmdiasduracao'][$key]:null;
                $_POST['icmvalor'][$key] = str_replace(',','.',str_replace('.','',$_POST['icmvalor'][$key]));
                $arrDados['icmvalor'] = ($_POST['icmvalor'][$key])?number_format($_POST['icmvalor'][$key],'2','.',''):null;
                $arrDados['icmpercentual'] = ($_POST['icmpercentual'][$key])?number_format($_POST['icmpercentual'][$key],'2','.',''):null;
                $arrDados['icmstatus'] = 'A';
                $arrDados['cpmid'] = $cpmid;
                $arrDados['icmid'] = ($_POST['icmid'][$key]) ? $_POST['icmid'][$key] : null;
                $arrDados['icmquantidade'] = ($_POST['icmquantidade'][$key])?$_POST['icmquantidade'][$key]:null;
                $arrDados['umdid'] = ($_POST['umdid'][$key])?$_POST['umdid'][$key]:null;
                $_POST['icmvalortotal'][$key] = str_replace(',','.',str_replace('.','',$_POST['icmvalortotal'][$key]));
                $arrDados['icmvalortotal'] = ($_POST['icmvalortotal'][$key])?number_format($_POST['icmvalortotal'][$key],'2','.',''):null;
                $arrDados['relativoEdificacao'] = ($_POST['relativoEdificacao'][$key]) ? $_POST['relativoEdificacao'][$key] : null;
                
                $arrDados['icmdiasduracao'] = ($arrDados['icmdiasduracao']) ? str_replace(',','',$arrDados['icmdiasduracao']) : null;
                
                $this->popularDadosObjeto($arrDados);
                $this->salvar();
                $this->commit();
            }
        }
        
        if($_POST['icmremovidos']){
            $icmids = explode(',',$_POST['icmremovidos']);
            foreach ($icmids as $icmid){
                $this->clearDados();
                $this->excluir($icmid);
                $this->commit();
            }
        }
        
    }

    public function excluir($icmid = null)
    {
        $icmid = ($icmid) ? $icmid : $_REQUEST['icmid'];

        $sql = "UPDATE {$this->stNomeTabela} SET icmstatus = 'I' WHERE icmid = $icmid";
        if($this->executar($sql)){
            $this->commit();
        }
    }

    public function excluirPorCpm($cpmid)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET icmstatus = 'I' WHERE cpmid = $cpmid";
        if($this->executar($sql)){
            $this->commit();
        }
    }
    
    
}
