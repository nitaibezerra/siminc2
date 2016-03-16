<?php

/**
 * Controle responsavel pelo historico de processos do par.
 * 
 * @author Equipe simec - Consultores OEI
 * @since  18/04/2013
 * 
 * @name       ProcessoItemAdesaoPregao
 * @package    classes
 * @subpackage controle
 */
class ProcessoItemAdesaoPregaoControle extends Controle
{
    /**
     * Array com os historicos do processo que nao foram importados,
     * contendo o id do historico e os dados da entidade de cada item do historico.
     * 
     * @name $_erroImportacao
     * @access protected
     * @var array
     */
    protected $_erroImportacao = array();
    
    /**
     * Array com os historicos do processo que nao foram importados.
     * 
     * @name $_erroImportacao
     * @access protected
     * @var array
     */
    protected $_sucessoImportacao = array();
    
    
    /**
     * Carrega a quantidade de historico de processos que ainda nao foram pulados e nao foram importados.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarHistoricoProcessoQtdRestante
     * @access public
     * @return integer $intQtdPulado - Quantidade de historicos que foram pulados na importacao
     */
    public function carregarHistoricoProcessoQtdRestante()
    {
        $sql = "SELECT count(*) FROM par.historicowsprocessopar where (hwpwebservice = 'enviarItensSigarp - Sucesso' OR hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso') and carga IS NULL AND pular IS NULL";
        $intQtdImportado = $this->db->pegaUm($sql);
        
        return $intQtdImportado;
    }
    
    /**
     * Carrega a quantidade de historico de processos que foram pulados na importacao.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarHistoricoProcessoQtdPulado
     * @access public
     * @return integer $intQtdPulado - Quantidade de historicos que foram pulados na importacao
     */
    public function carregarHistoricoProcessoQtdPulado()
    {
        $sql = "SELECT count(*)
                FROM par.historicowsprocessopar 
                where ( hwpwebservice = 'enviarItensSigarp - Sucesso' OR hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso' ) 
                and pular IS NOT NULL";
        $intQtdPulado = $this->db->pegaUm($sql);
        
        return $intQtdPulado;
    }
    
    /**
     * Carrega varios historico de processos de acordo com o limite de resultados.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarHistoricoProcesso
     * @access public
     * @param  integer $intLimit - Limite de resultados (Nao obrigatorio).
     * @return array $result - Retorna um array com os valores dos historico de processos
     */
    public function carregarHistoricoProcesso($intLimit = 500)
    {
        $sql = "SELECT * 
                FROM 
                    par.historicowsprocessopar 
                WHERE 
                    (hwpwebservice = 'enviarItensSigarp - Sucesso' OR hwpwebservice = 'enviarManualmenteItensSigarp - Sucesso') 
                AND 
                    carga IS NULL 
                    --AND hwpid = 1940242
                AND pular IS NULL
                ORDER BY 
                   hwpid DESC
                LIMIT 
                    {$intLimit}";
        $return = $this->db->carregar($sql);
        
        return $return;
    }
    
    /**
     * Carrega os historico do processo de acordo com o seu id principal.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarHistoricoProcessoPorId
     * @access public
     * @param  integer $hwpid - Id principal do historico do processo.
     * @return array $result - Retorna um array com os valores de um unico historico do processo
     */
    public function carregarHistoricoProcessoPorId($hwpid)
    {
        $sql = "SELECT * 
                FROM par.historicowsprocessopar 
                WHERE hwpid = {$hwpid}";
        $result = $this->db->pegaLinha($sql);
        
        return $result;
    }
    
    /**
     * Realiza importacao dos itens que estao no xml de um unico historico processo para outra tabela (par.processoitemadesaopregao).
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   transferirHistoricoProcesso
     * @access public
     * @param  integer $idHistoricoProcesso - Id do historico do processo à ser importado
     * @return array $idsPIAPPorHistorico - Retorna um array ids dos itens do historico do processo salvos com sucesso
     */
    public function transferirHistoricoProcesso($idHistoricoProcesso)
    {
        $historicoProcesso = $this->carregarHistoricoProcessoPorId($idHistoricoProcesso);
        $entitys = $this->popularEntidadePIAPPorHistorico($historicoProcesso);
        
        foreach($entitys as $entity){
            $idsPIAPPorHistorico[] = $this->salvarEntidadePIAPPorHistorico($entity);
        }
        $this->marcarHistoricoProcesso($idHistoricoProcesso);
        $this->db->commit();
        
        return $idsPIAPPorHistorico;
    }
    
    /**
     * Realiza importacao dos itens que estao no xml do historico processo para outra tabela (par.processoitemadesaopregao).
     * Se a importacao de um historico for realizada com sucesso, altera o campo carga para true, 
     * mostrando que todos os itens do historico processo foi salvos na nova tabela com sucesso.
     * Se a importacao dos intes de um historico tiver algum erro ele nao realiza a insersao dos itens dele.
     * Se o item de algum historico estiver faltando o icoid, marca na tabela de historico do processo o campo pular, mostrando que na proxima tentativa ira pular este historico.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   transferirVariosHistoricoProcesso
     * @access public
     * @param  integer $qtdImportacao - Quantidade a ser importado
     * @return array $return - Retorna um array com os historicos importados com sucesso e os que tiveram erro
     */
    public function transferirVariosHistoricoProcesso($qtdImportacao = null)
    {
        // Se tiver tido alguma transisao no banco anteriormente ele volta tudo.
        $this->db->rollback();
        
        //Carrega os historicos para serem importados de acordo com a quantidade necessitada.
        $historicoProcessos = $this->carregarHistoricoProcesso($qtdImportacao);
        if(is_array($historicoProcessos)){
	        foreach($historicoProcessos as $historicoProcesso){
	            
	            // Populando as entidade com cada item do historico do processo
	            $entitys = $this->popularEntidadePIAPPorHistorico($historicoProcesso);
	            
	            // Salva os itens de cada historico do processo e armazena os ids em um array caso tenha sucesso.
	            $ids = array();
	            foreach($entitys as $entity){
	                $id = $this->salvarEntidadePIAPPorHistorico($entity);
	                if($id) $ids[] = $id;
	            }
	            
	//            if($historicoProcesso['hwpid'] == 341540){
	//                ver(simec_htmlentities($historicoProcesso['hwpxmlenvio']), $entitys, $ids,d);
	//            }
	            
	            // Se a quantidade de entidades for igual a quantidade de ids, significa que todos os itens foram salvos com sucesso.
	            // Com isso marca o historico como realizado com sucesso, coloca o id do historico e os ids dos itens salvos no array de importacao com sucesso para gerar o log da importacao
	            // e realiza o comit com as transacoes.
	            if(count($entitys) == count($ids)){
	                $this->_sucessoImportacao[] = array('ID_HISTORICO' => $historicoProcesso['hwpid'],
	                                                    'ID_ITENS' => $ids);
	                $resultMarcarPocesso = $this->marcarHistoricoProcesso($historicoProcesso['hwpid']);
	                $this->db->commit();
	            } else {
	                
	                // Se não tiver a quantidade de entidades e ids ele verifica se o motivo foi por falta de icoid
	                $booPulou = false;
	                foreach($entitys as $entity){
	                    
	                    // Se nao tiver o icoid ele pula o historico
	                    if(!$entity->icoid){
	                        $booPulou = true;
	                        
	                    //Se um dos itens nao tiver valor do item, ele salva somente os itens que possui o valor e marca o historico do processo com carga = true,
	                    //significando que o historico foi importado com sucesso, mesmo faltando esse item sem o valor.
	                    } elseif(!$entity->piaquantidadeitens){
	                        $this->_sucessoImportacao[] = array('ID_HISTORICO' => $historicoProcesso['hwpid'],
	                                                    'ID_ITENS' => $ids);
	                        $resultMarcarPocesso = $this->marcarHistoricoProcesso($historicoProcesso['hwpid']);
	                        $this->db->commit();
	                    }
	                }
	                
	                
	                // Se nao tiver o icoid ele pula o historico marcando no banco o campo pular = true e comita
	                // Se nao for pra pular ele coloca o historico e os valores da entidade dos itens no array de impo
	                if($booPulou){
	                    $this->pularHistoricoProcesso($historicoProcesso['hwpid']);
	                    $this->db->commit();
	                } else {
	                    $this->_erroImportacao[] = array("ID_HISTORICO" => $historicoProcesso['hwpid'],
	                                                     'ITENS' => $entitys);
	                    $this->db->rollback();
	                }
	            }
        }
        }
        
        $return = array('sucesso' => $this->_sucessoImportacao, 'erro' =>$this->_erroImportacao);
        return $return;
        
    }
    
    /**
     * Realiza update na tabela de historico para marcar as importações que foram realizadas com sucesso.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   marcarHistoricoProcesso
     * @access public
     * @param  integer $hwpid - Id principal do historico para marcar o campo
     * @return boolean $result - Retorna sucesso ou falso
     */
    public function marcarHistoricoProcesso($id)
    {
        $sql = "UPDATE par.historicowsprocessopar SET carga = 'true' WHERE hwpid = {$id}";
        $result = $this->db->executar($sql);
        return $result;
    }
    
    /**
     * Realiza update na tabela de historico para marcar as importações que serão puladas, para serem corrigidas futuramente.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   pularHistoricoProcesso
     * @access public
     * @param  integer $hwpid - Id principal do historico para marcar o campo
     * @return boolean $result - Retorna sucesso ou falso
     */
    public function pularHistoricoProcesso($hwpid)
    {
        $sql = "UPDATE par.historicowsprocessopar SET pular = 'true' WHERE hwpid = {$hwpid}";
        $result = $this->db->executar($sql);
        return $result;
    }
    
    /**
     * Carrega o processo item adesao pregao de acordo com os parametros passado que são os ids.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarProcessoItemAdesaoPregaoPorIds
     * @access public
     * @param  integer $prpid - Id principal do processo
     * @param  integer $icoid - Id
     * @return array $row - Retorna a row com todos os campos da tabela
     */
    public function carregarProcessoItemAdesaoPregaoPorIds($prpid, $icoid)
    {
        $sql = "SELECT * FROM par.processoitemadesaopregao
                WHERE prpid = {$prpid}
                AND icoid = {$icoid}";
        $row = $this->db->pegaLinha($sql);
        return $row;
    }
    
    /**
     * Salva os valores da entidade processo item de adesao pregao.
     * Se ja tiver dados com os mesmos ids, realiza update.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   salvarEntidadePIAPPorHistorico
     * @access protected
     * @param  stdClass $entity - Entidade preenchida da tabela par.processoitemadesaopregao 
     * @return integer $id (piaid) - Id principal da entidade par.processoitemadesaopregao 
     */
    public function salvarEntidadePIAPPorHistorico(stdClass $entity)
    {
        // Validando entity caso algum valor esteja faltando ele retorna falso
        if(!$entity->prpid ||
           !$entity->icoid ||
           !$entity->pianumeroadesao ||
           !$entity->piaquantidadeitens ||
           !$entity->piavaloritens ||
           !$entity->piastatus)
            return false;
        
        // Carrega o item caso ja tenha algum salvo com os mesmos ids
        $processoItemAdesaoPregao = $this->carregarProcessoItemAdesaoPregaoPorIds($entity->prpid, $entity->icoid);
        
        // Montando a query
        // Se tiver algum item com os ids passado, realiza as alterações nos valores ja salvos
        // Se não insere os dados novos.
        if($processoItemAdesaoPregao){
            $sql = "UPDATE par.processoitemadesaopregao 
                    SET 
                        prpid = {$entity->prpid} ,
                        icoid = {$entity->icoid} ,
                        pianumeroadesao = '{$entity->pianumeroadesao}' ,
                        piaquantidadeitens = '{$entity->piaquantidadeitens}' ,
                        piavaloritens = '{$entity->piavaloritens}' ,
                        piastatus = '{$entity->piastatus}'
                    WHERE 
                        piaid ={$processoItemAdesaoPregao['piaid']}
                    RETURNING piaid";
                        
        } else {
            $sql = "INSERT INTO par.processoitemadesaopregao 
                    (prpid, icoid , pianumeroadesao , piaquantidadeitens , piavaloritens , piastatus)
                    VALUES (
                        {$entity->prpid} ,
                        {$entity->icoid} ,
                        '{$entity->pianumeroadesao}' ,
                        '{$entity->piaquantidadeitens}' ,
                        '{$entity->piavaloritens}' ,
                        '{$entity->piastatus}'
                            ) 
                    RETURNING piaid";
        }
        $id = $this->db->pegaUm($sql);
        return $id;
    }
    
    /**
     * Realiza tratamento para caso o id do tipo de item seja antigo ele muda para o id do novo tipo de item.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   tratarNuSeqItem
     * @access protected
     * @param  integer $nuSeqItem - Id do item
     * @return void
     */
    protected function tratarNuSeqItem(&$nuSeqItem)
    {
        switch ($nuSeqItem):
            case 5:
                $nuSeqItem = 1801;
                break;
            case 1:
                $nuSeqItem = 1802;
                break;
            case 2:
                $nuSeqItem = 1803;
                break;
            case 3:
                $nuSeqItem = 1804;
                break;
            case 6:
                $nuSeqItem = 1805;
                break;
            case 4:
                $nuSeqItem = 1806;
                break;
            case 7:
                $nuSeqItem = 1807;
                break;
            default :
                break;
        endswitch;
    }
    
    /**
     * Retorna o id de acordo com os parametros passado.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   carregarIcoidPorPrpidEIdsigarp
     * @access protected
     * @param  integer $prpid    - Id principal do processo
     * @param  integer $idsigarp - Id prinsipal do sigarp
     * @return integer $icoid - Id
     */
    public function carregarIcoidPorPrpidEIdsigarp($prpid, $idsigarp)
    {
        
        $sql = "SELECT icoid

                FROM par.processopar p

                INNER JOIN par.processoparcomposicao pc ON pc.prpid = p.prpid and pc.ppcstatus = 'A'

                INNER JOIN par.subacaodetalhe sd ON sd.sbdid = pc.sbdid

                INNER JOIN par.subacaoitenscomposicao  si ON si.sbaid = sd.sbaid and si.icoano = sd.sbdano

                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid

                INNER JOIN par.subacao s ON s.sbaid = sd.sbaid

                WHERE p.prpid = '{$prpid}' and pic.idsigarp = {$idsigarp}
                AND p.prpstatus = 'A' ";
        $icoid = $this->db->pegaUm($sql);
        
        if( !$icoid ){
        	$sql = "SELECT icoid FROM par.processopar p
					INNER JOIN par.pontuacao po ON p.inuid = po.inuid  AND po.ptostatus = 'A'
					INNER JOIN par.acao a on a.ptoid = po.ptoid
					INNER JOIN par.subacao s ON s.aciid = a.aciid
					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
	                INNER JOIN par.subacaoitenscomposicao  si ON si.sbaid = sd.sbaid and si.icoano = sd.sbdano
	                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
	                WHERE p.prpid = '{$prpid}' and pic.idsigarp = {$idsigarp} 
	                AND p.prpstatus = 'A'
	                ";
        	
	        $icoid = $this->db->pegaUm($sql);
        }
        
        return $icoid;
    }
    
    /**
     * Popula as entidades do com os itens de cada historico, que estao dentro do xml.
     * Feito tratamentos para campos com o formato diferente do xml de envio.
     * Feito tratamentos para campos com o formato diferente do xml de retorno.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   popularEntidadePIAPPorHistorico
     * @access protected
     * @param  array $historicoProcesso - Array com os historicos do processo
     * @return array $entitys - Array de entidades que sao objetos
     * 
     */
    protected function popularEntidadePIAPPorHistorico($historicoProcesso)
    {
        // Recupera o xml de envio transformando em objetos
        $xmlEnvio = simplexml_load_string($historicoProcesso['hwpxmlenvio'])->children();
        
        // Recupera o xml de retorno transformando em objetos
        $xmlRetorno = simplexml_load_string($historicoProcesso['hwpxmlretorno'])->children();
        
        // Comeca o processo de popular as entidades de acordo com os itens do xml
        $entitys = array();
        if($xmlEnvio->body->params->sub_acao->item){
            
            // Realiza loop com os itens do historico do processo
            foreach($xmlEnvio->body->params->sub_acao->item as $item ){
                
                // Recupera o idsigarp e se for o valor antigo realiza o tratamento para o valor novo
                $nuSeqItem = reset($item->nu_seq_item);
                $this->tratarNuSeqItem($nuSeqItem);
                
                // Recupera o valor do item e realiza tratamento para o formato do banco que e numerico
                $valorItem = reset($item->vl_pagamento);
                $valorItem = str_replace('.', '', $valorItem);
                $valorItem = str_replace(',', '.', $valorItem);
                
                // Recupera o icoid pelo historico e id do sigarp
                $icoid = $this->carregarIcoidPorPrpidEIdsigarp($historicoProcesso['prpid'], $nuSeqItem);
                
                // Recupera numero de adesao do xml de retorno se nao for de um formato o xml tenta pegar de outro formato xml
                $nuAdesao = reset($xmlRetorno->body->params->adesaopregao->nu_seq_solicitacao_adesao);
                if(!$nuAdesao) $nuAdesao = reset($xmlRetorno->status->message->code);
                
                // Populando a entidade com os dados ja preparados
                $entity = $this->entityProcessoItemAdesaoPregao();
                $entity->prpid = $historicoProcesso['prpid'];
                $entity->icoid = $icoid;
                $entity->pianumeroadesao = $nuAdesao;
                $entity->piaquantidadeitens = reset($item->qt_item);
                $entity->piavaloritens = $valorItem;
                $entity->piastatus = "A";
                $entitys[] = $entity;
            }
        } else {
            
            foreach($xmlEnvio->body->params->item as $item ){
                
                // Recupera o idsigarp e se for o valor antigo realiza o tratamento para o valor novo
                $nuSeqItem = reset($item->nu_seq_item);
                $this->tratarNuSeqItem($nuSeqItem);
                
                // Recupera o icoid pelo historico e id do sigarp
                $icoid = $this->carregarIcoidPorPrpidEIdsigarp($historicoProcesso['prpid'], $nuSeqItem);
                
                // Recupera o valor do item e realiza tratamento para o formato do banco que e numerico
                $valorItem = reset($item->vl_pagamento);
                $valorItem = str_replace('.', '', $valorItem);
                $valorItem = str_replace(',', '.', $valorItem);
                
                // Recupera numero de adesao do xml de retorno se nao for de um formato o xml tenta pegar de outro formato xml
                $nuAdesao = reset($xmlRetorno->body->params->adesaopregao->nu_seq_solicitacao_adesao);
                if(!$nuAdesao) $nuAdesao = reset($xmlRetorno->status->message->code);
                
                // Populando a entidade com os dados ja preparados
                $entity = $this->entityProcessoItemAdesaoPregao();
                $entity->prpid = $historicoProcesso['prpid'];
                $entity->icoid = $icoid;
                $entity->pianumeroadesao = $nuAdesao;
                $entity->piaquantidadeitens = reset($item->qt_item);
                $entity->piavaloritens = $valorItem;
                $entity->piastatus = "A";
                $entitys[] = $entity;
            }
        }
        
        return $entitys;
    }
    
    /**
     * Objetos que representa a entidade par.processoitemadesaopregao.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  18/04/2013
     * 
     * @name   entityProcessoItemAdesaoPregao
     * @access public
     * @return \stdClass - Retorna em objetos que representa a entidade par.processoitemadesaopregao
     */
    public function entityProcessoItemAdesaoPregao()
    {
        $entity = new stdClass();
        
        $entity->piaid = "";
        $entity->prpid = "";
        $entity->icoid = "";
        $entity->pianumeroadesao = "";
        $entity->piaquantidadeitens = "";
        $entity->piavaloritens = "";
        $entity->piastatus = "";
        
        return $entity;
    }
}
