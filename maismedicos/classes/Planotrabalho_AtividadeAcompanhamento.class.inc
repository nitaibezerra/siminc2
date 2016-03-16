<?php

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';
	
class Planotrabalho_atividadeacompanhamento extends Planotrabalho{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.planotrabalho_atividadeacompanhamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptaid' => null, 
									  	'ptrid' => null, 
									  	'pteid' => null, 
									  	'uniid' => null, 
									  	'ifeid' => null, 
									  	'ifcid' => null, 
									  	'arqid' => null, 
									  	'ptanome' => null, 
									  	'ptatipo' => null, 
									  	'ptadtprevisto' => null, 
									  	'ptadsc' => null, 
									  	'ptaexecutado' => null, 
									  	'arqidexecutado' => null, 
									  	'ptadtexecutado' => null, 
									  	'ptaobsexecutado' => null, 
									  	'ptadtenvioexec' => null, 
									  	'ptastatus' => null, 
									  );
    
    public function salvaAtividadeAcompanhamentoPT($ptrid = null, $post = null, $files = null)
    {
    	if($ptrid>0){
    
    		if($post['ifeid_atividade']){
    			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    			$file = new FilesSimec("planotrabalho_atividadeacompanhamento", array(''), "maismedicos");
    			foreach($post['ifeid_atividade'] as $k => $v){

    				$arDados['ptaid'] = $post['ptaid'][$k];
    				if(empty($post['ptaid'][$k])){
	    				$arDados['ptrid'] = $ptrid;
    				}
    				if(empty($post['ptadtenvioexec'][$k])){
    					$arDados['pteid'] = $post['pteid'];
    				}
    				if($post['ifeid_atividade'][$k])
    					$arDados['ifeid'] = $post['ifeid_atividade'][$k];
    				
    				if($post['ifcid_atividade'][$k])
    					$arDados['ifcid'] = $post['ifcid_atividade'][$k];
    				
    				$arDados['ptanome'] = $post['ptanome'][$k];
    				$arDados['ptatipo'] = $post['ptatipo'][$k];
    				$arDados['ptadsc'] = $post['ptadsc'][$k];
    				
    				if(!empty($post['ptadtprevisto'][$k]))
    					$arDados['ptadtprevisto'] = formata_data_sql($post['ptadtprevisto'][$k]);
    				
    				if(!empty($post['ptadtexecutado'][$k]))
    					$arDados['ptadtexecutado'] = formata_data_sql($post['ptadtexecutado'][$k]);
    				
    				if(!empty($post['ptaobsexecutado'][$k]))
    					$arDados['ptaobsexecutado'] = $post['ptaobsexecutado'][$k];
    				
    				if($post['enviar_execucao']=='true' && empty($post['ptadtenvioexec'][$k]) && !empty($post['ptadtexecutado'][$k]))
    					$arDados['ptadtenvioexec'] = date('Y-m-d');
    					//ver($arDados, d);
    				if($files['arquivo_atividade']['name'][$k]){
    
    					$_FILES['arquivo']['name'] = $files['arquivo_atividade']['name'][$k];
    					$_FILES['arquivo']['type'] = $files['arquivo_atividade']['type'][$k];
    					$_FILES['arquivo']['tmp_name'] = $files['arquivo_atividade']['tmp_name'][$k];
    					$_FILES['arquivo']['error'] = $files['arquivo_atividade']['error'][$k];
    					$_FILES['arquivo']['size'] = $files['arquivo_atividade']['size'][$k];
    
    					$file->setUpload(null, "arquivo", false);
    					$arDados['arqid'] = $file->getIdArquivo();
    				}
    				if($files['arquivo_atividade_execucao']['name'][$k]){
    
    					$_FILES['arquivo']['name'] = $files['arquivo_atividade_execucao']['name'][$k];
    					$_FILES['arquivo']['type'] = $files['arquivo_atividade_execucao']['type'][$k];
    					$_FILES['arquivo']['tmp_name'] = $files['arquivo_atividade_execucao']['tmp_name'][$k];
    					$_FILES['arquivo']['error'] = $files['arquivo_atividade_execucao']['error'][$k];
    					$_FILES['arquivo']['size'] = $files['arquivo_atividade_execucao']['size'][$k];
    
    					$file->setUpload(null, "arquivo", false);
    					$arDados['arqidexecutado'] = $file->getIdArquivo();
    				}
    				//ver($post['ptadtenvioexec'][$k], $arDados['arqidexecutado'], $files['arquivo_atividade_execucao']['name'][$k], d);
    				
    				if(empty($post['ptadtenvioexec'][$k])){
    				$this->popularDadosObjeto($arDados);
    				$ptaid = $this->salvar();
    				$this->commit();
    				$this->clearDados();
    				if($ptaid>0) $arIds[] = $ptaid;
    				}
    				unset($arDados);
    			}
    			if($arIds){
	    			$this->executar("update maismedicos.planotrabalho_atividadeacompanhamento set ptastatus = 'I' where ptrid = {$ptrid} and ptaid not in (".implode(',',$arIds).")");$this->commit();
    			}
    		}
    	}
    }
    
}