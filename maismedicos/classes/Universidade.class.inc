<?php
class Universidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.universidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "uniid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	  'uniid' => null,
										  'entid' => null,
										  'funid' => null,
										  'uninome' => null,
    									  'unicnpj' => null,
										  'unisigla' => null,
										  'estuf' => null,
										  'muncod' => null,
										  'unidatacadastro' => null,
										  'uniusucpfcadastro' => null,
										  'unidataatualizacao' => null,
										  'uniusucpfatualizacao' => null,
										  'unistatus' => null,
    									  'tpuid' => null
									  );
    
    public function listarUniversidades()
    {
    	
    	if($_POST['uniid']){
    		$arrWhere[] = "uni.uniid = {$_POST['uniid']}";
    	}
    	if($_POST['estuf']){
    		$arrWhere[] = "mun.estuf = '{$_POST['estuf']}'";
    	}
    	if($_POST['muncod']){
    		$arrWhere[] = "mun.muncod = '{$_POST['muncod']}'";
    	}
    	
    	if($_POST['preadesao'] == "sim"){
    		$arrWhere[] = "mai.uniid is not null";
    	}
    	
    	if($_POST['preadesao'] == "nao"){
    		$arrWhere[] = "mai.uniid is null";
    	}
    	
    	if($_POST['adesao'] == "sim"){
    		$arrWhere[] = "msmadesao is true";
    	}
    	
    	if($_POST['adesao'] == "nao"){
    		$arrWhere[] = "msmadesao is null";
    	}
    	
    	$arrPerfil = pegaPerfilGeral();
    	
    	if(( in_array(PERFIL_REITOR,$arrPerfil) || 
    			in_array(PERFIL_APOIADOR,$arrPerfil) || 
    			in_array(PERFIL_TUTOR,$arrPerfil) || 
    			in_array(PERFIL_SUPERVISOR,$arrPerfil) ) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil)){
    		$arrWhere[] = "ure2.usucpf = '{$_SESSION['usucpf']}'";
    	}
    	
    	$sql = "select distinct
    				'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarTermoAdesao(' || uni.uniid || ')\"   />' as acao,
    				uninome,
    				coalesce(unisigla,'N/A') as sigla,
    				tpudsc,
    				mun.estuf,
    				mundescricao,
    				coalesce(usu.usunome,'N/A') as reitor,
    				to_char(unidataatualizacao,'DD/MM/YYYY HH:MM:SS') as data,    				
    				case when msmadesao is true 
    					then 'Sim'
    					else 'Não'
    				end as termoadesao
    			from
    				$this->stNomeTabela uni
    			inner join
    				 maismedicos.tipouniversidade tpu ON tpu.tpuid = uni.tpuid
    			".($arrJoin ? implode(" ",$arrJoin) : "")."
    			left join
    				maismedicos.usuarioresponsabilidade ure2 ON ure2.uniid = uni.uniid 
    						and ure2.pflcod in (".implode(',',$arrPerfil).") 
    								and ure2.rpustatus = 'A'
    			left join
    				seguranca.usuario usu ON usu.usucpf = ure2.usucpf
    			left join
    				maismedicos.maismedicos mai ON mai.uniid = uni.uniid
    			inner join
    				territorios.municipio mun ON mun.muncod = uni.muncod
    			where
    				unistatus = 'A'
    				".($arrWhere ? "and ".implode(" and ",$arrWhere) : "")."
    			order by
    				sigla,
    				uninome";
//     	ver($sql);
    	$arrCab = array("Ação","Instituição Supervisora","Sigla","Tipo","UF","Município","Responsável","Última Atualização","Adesão?");
    	$this->monta_lista($sql, $arrCab,100, 10, "N", "center");
    }
    
    public function comboUniversidades()
    {
    	$uniid = $_POST['uniid'];
    	
    	$arrPerfil = pegaPerfilGeral();
    	 
    	if(in_array(PERFIL_REITOR,$arrPerfil) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil)){
    		$arrJoin[] = "inner join maismedicos.usuarioresponsabilidade ure ON ure.uniid = uni.uniid and usucpf = '{$_SESSION['usucpf']}' and pflcod = ".PERFIL_REITOR;
    	}
    	
    	$sql = "select distinct
		    		uni.uniid as codigo,
		    		case when unisigla is not null then 
		    			unisigla || ' - ' || uninome 
		    			else uninome
		    		end as descricao
		    	from
		    		$this->stNomeTabela uni
		    	".($arrJoin ? implode(" ",$arrJoin) : "")."
		    	where
		    		unistatus = 'A'
		    	order by
		    		descricao";
    	$this->monta_combo("uniid", $sql, "S", "Todas as Instituições", '', '', '', '', 'N','uniid');
    }
    
    public function carregarNomeReitor()
    {
    	$usucpf = $_POST['usucpf'];
    	if($usucpf){
    		$usucpf = str_replace(array(".","-"),array("",""),$usucpf);
    		$sql = "select usunome from seguranca.usuario where usucpf = '$usucpf'";
    		$usunome = $this->pegaUm($sql);
    		if($usunome){
    			echo $usunome;
    		}else{
    			echo "Este CPF não está cadastrado no sistema.";
    		}
    	}else{
    		echo "";
    	}
    }
    
    public function salvarUniversidade()
    {
    	$_POST['unicnpj'] = str_replace(array(".","-","/"),array("","",""),$_POST['unicnpj']);
    	$this->popularDadosObjeto($_POST);
    	$this->salvarReitor($_POST['uniid'],$_POST['usucpfreitor']);
    	$uniid = $this->salvar();
    	if($uniid){
    		$this->commit();
    		$_SESSION['maismedicos']['universidade']['alert'] = "Operação realizada com sucesso.";
    	}else{
    		$_SESSION['maismedicos']['universidade']['alert'] = "Não foi possível realizar a operação.";
    	}
    	header("Location: maismedicos.php?modulo=principal/dadosInstituicao&acao=A");
    	exit;
    }
    
    public function recuperaReitor($uniid = null)
    {
    	$uniid = $uniid ? $uniid : $this->uniid;
    	if($uniid){
    		$sql = "select 
    					usu.usucpf,
    					usu.usunome 
    				from 
    					maismedicos.usuarioresponsabilidade ure
    				inner join
    					seguranca.usuario usu ON usu.usucpf = ure.usucpf
    				where 
    					ure.uniid = $uniid 
    				and 
    					ure.pflcod = ".PERFIL_REITOR." 
    				and 
    					ure.rpustatus = 'A' 
    				order by 
    					ure.rpudata_inc desc";
    		return $this->pegaLinha($sql);
    	}else{
    		return false;
    	}
    }
    
    public function salvarReitor($uniid,$usucpf)
    {
    	$usucpf = str_replace(array(".","-"),array("",""),$usucpf);
    	$sql = "select usucpf from seguranca.usuario where usucpf = '$usucpf'";
    	if(!$this->pegaUm($sql)){
    		return false;
    	}
    	
    	$sql = "update maismedicos.usuarioresponsabilidade set rpustatus = 'I' where uniid = $uniid and pflcod = ".PERFIL_REITOR;
    	$this->executar($sql);
    	
    	$sql = "insert into maismedicos.usuarioresponsabilidade (pflcod,usucpf,uniid) values (".PERFIL_REITOR.",'$usucpf',$uniid)"; 
    	$this->executar($sql);
    	return $this->commit();
    }
    
}