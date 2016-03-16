<?php
	
class Municipio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.municipio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "muncod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'miccod' => null, 
									  	'mescod' => null, 
									  	'mundescricao' => null, 
									  	'munprocesso' => null, 
									  	'muncodcompleto' => null, 
									  	'munmedlat' => null, 
									  	'munmedlog' => null, 
									  	'munhemis' => null, 
									  	'munaltitude' => null, 
									  	'munmedarea' => null, 
									  	'muncepmenor' => null, 
									  	'muncepmaior' => null, 
									  	'munmedraio' => null, 
									  	'munmerid' => null, 
									  	'muncodsiafi' => null, 
									  	'munpopulacao' => null, 
									  );
									  
	public function lista($arInner = array(), $arWhere = array(), $exporta = null)
	{
		$link = " '' ";
		$perfis = pegaPerfil($_SESSION['usucpf']);
		if(is_array($perfis)){
			$link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalhoM(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";	
			foreach($perfis as $perifl){
				if( $perifl == PAR_PERFIL_SUPER_USUARIO || $perfil == PAR_PERFIL_ADMINISTRADOR ){
					$link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
					break;
				}
			}
		}else if($perfis !== NULL){
			if($perfis == PAR_PERFIL_EQUIPE_MUNICIPAL ){
				$link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalhoM(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
			}else{
				$link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
			}
		}
		
		if($exporta == "true"){
			$acaoBtn = "";
		} else {
			$acaoBtn = $link." as acao,";
		}
		$sql = "select distinct ".$acaoBtn." 
				-- m.muncod, m.mundescricao, m.estuf, ed.esddsc
				m.muncod, m.mundescricao, m.estuf, CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE ed.esddsc END as sit
		from territorios.municipio m
		LEFT JOIN par.instrumentounidade iu ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
		LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid 	= 44 
		LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
		{$arInner['join_capitais']}
		{$arInner['join_grupo']}
		-- {$arInner['join_grandescidades']}
		-- {$arInner['join_indigena']}
		-- {$arInner['join_quilombola']}
		-- {$arInner['join_cidadania']}
		{$arInner['join_ideb']}
		{$arInner['join_dadosunidade']}
		" . ( is_array($arWhere) && count($arWhere) ? ' where ' . implode(' AND ', $arWhere) : '' ) ."
		order by m.mundescricao
		";

		$cabecalho = array("Ação","Código","Município", "UF", "Situação");
		$tamanho   = array( '5%', '10%', '50%', '10%', '10%' );
		
		if($exporta == "true"){
			return $sql;
		}else{
			$this->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2,'',$tamanho);
		}
		
		
	
	}
	
	public function descricaoMunicipio($muncod, $boMostraEstuf = true)
	{
		if($boMostraEstuf){
			return $this->pegaUm("SELECT estuf || ' - ' || mundescricao FROM territorios.municipio where muncod = '$muncod' ");
		} else {
			return $this->pegaUm("SELECT mundescricao FROM territorios.municipio where muncod = '$muncod' ");
		}
	}
	
	public function recuperarUF($muncod)
	{
		$sql = "SELECT estuf FROM {$this->stNomeTabela} WHERE muncod = '{$muncod}'";
		return $this->pegaUm($sql);
	}
	
}