<?php
	
class Projeto extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.projeto";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prjid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prjid' => null, 
									  	'ungcod' => null, 
									  	'prjnome' => null, 
									  	'prjsigla' => null, 
									  	'prjdesc' => null, 
									  	'prjescopo' => null, 
									  	'prjobs' => null, 
									  	'prjdt' => null,
    									'ctrid' => null
									  );
	
	protected $arrPesquisa = null;

	public function showAnexosProjeto($prjid = null)
	{
		if($prjid):
			$sql = sprintf("select
								'<center><img class=\"middle link\" title=\"Download Anexo\" alt=\"Download Anexo\" onclick=\"download(\'' || arq.arqid || '\')\" src=\"../imagens/anexo.gif\" /> <img class=\"middle link\" title=\"Excluir Anexo\" alt=\"Exluir Anexo\" onclick=\"excluirAnexo(\'' || prjid || '\',\'' || arq.arqid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
								 to_char(arqdata,'DD-MM-YYYY') as datainclusao,
								 arqtipo as tipoarquivo,
								 '<a href=\"javascript:download(\'' || arq.arqid || '\')\">' || arqnome || '</a>' as nomearquivo,
								 (arqtamanho/1024) as tamanho,
								 arqdescricao as descricao,
								 (select usu.usunome from seguranca.usuario usu where arq.usucpf = usu.usucpf limit 1) as responsavel
							from
								fabrica.anexoprojeto ane
							inner join
								public.arquivo arq ON arq.arqid = ane.arqid
							where
								prjid = %d",
						(int)$prjid);
		else:
			$sql = array();
		endif;
		
		$cabecalho = array("Ações","Data da Inclusão","Tipo do Arquivo","Nome do Arquivo","Tamanho (Mb)","Descrição","Responsável");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function showModulosProjeto($prjid = null)
	{
		if($prjid):
			$sql = sprintf("select
								mdpid,
								prjid,
								mdpdsc
							from
								fabrica.moduloprojeto
							where
								prjid = %d
							and
								mdpstatus = 'A'",
						(int)$prjid);
			$arrModulos = $this->carregar($sql);
		else:
			$arrModulos = false;
		endif;
		
		$num = 1;
		
		if($arrModulos):
			foreach($arrModulos as $modulo):
				?>
				<tr class="modulos" id="tr_modulo_<?=$num?>"  >
					<td class="SubtituloDireita">Módulo <?=$num?>:</td>
					<td><?=campo_texto("arrModulo[".$modulo['mdpid']."]","N","S","Adicionar Módulo","40","120","","","","","","","",$modulo['mdpdsc']) ?> <img src="../imagens/excluir.gif" class="middle link" title="Excluir Módulo" onclick="excluirModulo('<?=$modulo['mdpid']?>','<?=$num?>')"></td>
				</tr>
				<?php
				$num++;
			endforeach;
		else:
		endif;
	}
	
	public function showListaProjetos()
	{
				
		$sql = "select
					'<center><img class=\"middle link\" title=\"Alterar Projeto\" alt=\"Alterar Projeto\" onclick=\"alterarProjeto(\'' || prj.prjid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Projeto\" alt=\"Excluir Projeto\" onclick=\"excluirProjeto(\'' || prj.prjid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					prj.prjid,
					(select ctrnumero from fabrica.contrato ctr where ctr.ctrid = prj.ctrid) as contrato,
					(select ungdsc from public.unidadegestora where ungcod = prj.ungcod) as unidade,
					prjnome,
					prjsigla,
					prjdesc,
					prjescopo,
					prjobs, 
					to_char(prjdt,'DD-MM-YYYY') as datainclusao
				from
					fabrica.projeto prj
				where
					prjstatus = 'A'
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
				order by
					prjnome";

		$cabecalho = array("Ações","Código","Nº Contrato","Unidade Gestora","Nome do Projeto","Sigla","Descrição","Escopo","Observações","Data de Criação");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function excluirProjeto()
	{
		$prjid = $_REQUEST['prjid'];
		
		$sql = sprintf("update fabrica.projeto set prjstatus = 'I' where prjid = %d",(int)$prjid);
		$this->executar($sql);
		$this->commit($sql);
		
	}
	
	public function pesquisarProjeto()
	{
		foreach($this->arAtributos as $campos => $valor){
			if($_POST[$campos] && $_POST[$campos] != ""){
				$this->arrPesquisa[] = $campos."::text ilike ('%".$_POST[$campos]."%') ";
			}
		}
		
	}
		
}