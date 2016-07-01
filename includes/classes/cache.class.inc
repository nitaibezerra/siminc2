<?php
/*
 * Classe Cache
 * Classe para fazer cache em arquivo
 * @author Juliano Meinen
 * @since 28/11/2011
 */
class Cache extends cls_banco{

	public $diretorio; 
	public $mnuid;
	private $usucpf = null;
	public $arrInfo = array();
	private $naoFazerCache = false;
	
	public function __construct($mnuid = null ,$usucpf = null)
	{
		parent::__construct();
		
		if(!$mnuid){
			$this->mnuid = $_SESSION['mnuid'];	
		}else{
			$this->mnuid = $mnuid;
		}
		if($this->verificarMnuid()){
			if($usucpf){
				$this->usucpf = $usucpf;
			}
			
			if( !$this->recuperarCache() ){
				register_shutdown_function(array($this,criarCache));
			}
		}
	}

	public function verificarMnuid()
	{
		$sql = "select * from seguranca.cache where mnuid = ".$this->mnuid." and cacstatus = 'A'";
		$arrDados = $this->pegaLinha($sql);
		if($arrDados){
			$this->cacid 		 	= $arrDados['cacid'];
			$this->mnuid 		 	= $arrDados['mnuid'];
			$this->cactipoexpira 	= $arrDados['cactipoexpira'];
			$this->cacexpira 	 	= $arrDados['cacexpira'];
			$this->cacrefazercache  = $arrDados['cacrefazercache']  == "t" ? "true" : "false";
			$this->cacnaofazercache = $arrDados['cacnaofazercache'] == "t" ? "true" : "false";
			return true;
		}else{
			return false;
		}
	}
	
	/**
	 * Método para criação do chache.
	 * @access public
	 * @author Juliano Meinen
	 * @since 28/11/2011
	 * @version 0.1
	 */
	public function criarCache()
	{
		
		if($this->naoFazerCache){
			$this->arrInfo[] = "Existe um parametro no banco de dados que não deixa o cache ser gerado para esta página.";
			return false;
		}
		
		$arrSessao = $this->tratarSessao();
		if(is_array($arrSessao)){ // Loop no $_SESSION tratado para gerar a hash do cache
			foreach($arrSessao as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_POST)){ // Loop no $_POST para gerar a hash do cache
			foreach($_POST as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_GET)){ // Loop no $_GET para gerar a hash do cache
			foreach($_GET as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(!$this->diretorio){ //Verifica se não existe diretorio para o cache
			$this->diretorio = APPRAIZ."arquivos/".$_SESSION['sisdiretorio'];
		}
		$cacheFile = md5($string); //Nome do arquivo que armazenará o conteúdo.
		$cacheConteudo = ob_get_contents(); //Obtém o conteúdo
		if(!is_dir($this->diretorio)){//Verifica se não existe o diretório para gravação do cache
			mkdir($this->diretorio, 0777); //Cria o diretório para armazenamento do cache com permissão para escrita
		}
		if(!is_dir($this->diretorio."/cache")){//Verifica se não existe o diretório 'cache' para gravação dos arquivos
			mkdir($this->diretorio."/cache", 0777); //Cria o diretório para armazenamento do cache com permissão para escrita
		}
		if(!is_dir($this->diretorio."/cache/".$this->mnuid)){//Verifica se não existe o diretório com o 'mnuid' do menu para gravação dos arquivos
			mkdir($this->diretorio."/cache/".$this->mnuid, 0777); //Cria o diretório para armazenamento do cache com permissão para escrita
		}
		if($this->usucpf){ // Cache individual por usuário
			if(!is_dir($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf)){//Verifica se não existe o diretório com o 'cpf do usuário' do menu para gravação dos arquivos
				mkdir($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf, 0777); //Cria o diretório para armazenamento do cache com permissão para escrita
			}
			$cacheArquivo = fopen($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf."/$cacheFile.html", "a"); //Cria um arquivo html para armazenar o conteúdo 
			fwrite($cacheArquivo, $cacheConteudo); // Escreve o conteúdo no arquivo criado
			fclose($cacheArquivo); //Fechar o arquivo
		}else{
			$cacheArquivo = fopen($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html", "a"); //Cria um arquivo html para armazenar o conteúdo 
			fwrite($cacheArquivo, $cacheConteudo); // Escreve o conteúdo no arquivo criado
			fclose($cacheArquivo); //Fechar o arquivo
			
			$this->atualizaCacheParametro();			
		}
	}
	
	public function atualizaCacheParametro()
	{
		$parametros['SESSION'] = $this->tratarSessao();
		$parametros['POST'] = $_POST;
		$parametros['GET'] = $_GET;
		$caprequisicao = simec_json_encode($parametros);
		
		$sql = "select capid from seguranca.cacheparametro where cacid = ".$this->cacid." and caprequisicao = '$caprequisicao' and capstatus = 'A'";
		$capid = $this->pegaUm($sql);
		if($capid){
			$sql = "update
						seguranca.cacheparametro
					set
						capdata = '".date("Y-m-d H:i:s")."'
					where
						capid = $capid";
		}else{
			$sql = "insert into
						seguranca.cacheparametro
					(cacid,caprequisicao,captipoexpira,capexpira,capdata,caprefazercache,capnaofazercache,capstatus)
						values
					(".$this->cacid.",'$caprequisicao','".$this->cactipoexpira."',".$this->cacexpira.",'".date("Y-m-d H:i:s")."',".$this->cacrefazercache.",".$this->cacnaofazercache.",'A')";
		}
		$this->executar($sql);
		$this->commit();
	}
	
	/**
	 * Método para recuperação do chache.
	 * @access public
	 * @author Juliano Meinen
	 * @since 28/11/2011
	 * @version 0.1
	 */
	public function recuperarCache()
	{
		
		if(!$this->verificaCacheParametro()){
			return false;
		}
		
		if($this->naoFazerCache){
			$this->arrInfo[] = "Existe um parametro no banco de dados que não deixa o cache ser gerado para esta página.";
			return true;
		}
		
		$arrSessao = $this->tratarSessao();
		if(is_array($arrSessao)){ // Loop no $_SESSION tratado para gerar a hash do cache
			foreach($arrSessao as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_POST)){ // Loop no $_POST para gerar a hash do cache
			foreach($_POST as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_GET)){ // Loop no $_GET para gerar a hash do cache
			foreach($_GET as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(!$this->diretorio){ //Verifica se não existe diretorio para o cache
			$this->diretorio = APPRAIZ."arquivos/".$_SESSION['sisdiretorio'];
		}
		
		$cacheFile = md5($string); //Nome do arquivo que armazenará o conteúdo.
		
		if($this->usucpf){ // Cache individual por usuário
			if(is_file($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf."/$cacheFile.html")){
				$this->tratarRodape($cacheFile);
				include($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf."/$cacheFile.html");
				$this->arrInfo[] = "A página está utilizando cache.";
				exit;
			}else{
				$this->arrInfo[] = "A página não está utilizando cache.";
				return false;
			}
		}else{
			if(is_file($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html")){
				$this->tratarRodape($cacheFile);
				include($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html");
				$this->arrInfo[] = "A página está utilizando cache.";
				exit;
			}else{
				$this->arrInfo[] = "A página não está utilizando cache.";
				return false;
			}
		}
	}
	
	/**
	 * Método para atualizar os dados do rodapé do SIMEC
	 * @access private
	 * @author Juliano Meinen
	 * @since 29/11/2011
	 * @version 0.1
	 */
	private function tratarRodape($cacheFile)
	{
		if($this->usucpf){ // Cache individual por usuário
			$cacheArquivo = file ($this->diretorio."/cache/".$this->mnuid."/".$this->usucpf."/$cacheFile.html");
		}else{
			$cacheArquivo = file ($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html");
		}
		
		$html = implode ('',$cacheArquivo);
		
		$pos_inicial_cab = 0;
		$pos_final_cab = strpos($html,"class=\"tbl_conteudo");
		if($pos_final_cab){
			$html = substr($html,$pos_final_cab + 331,strlen($html));
			global $db;
			include(APPRAIZ."includes/cabecalho.inc");
			echo "<br />";
		}
		//dbg($pos_final_cab,1);
		$pos_inicial = strpos($html,"<table class=\"rodape\"");
		$pos_final = strpos($html,"<div id=\"avisochat");
		if($pos_inicial && $pos_final){
			$antigoHtml = substr($html,$pos_inicial,$pos_final);
			$novoHtml = substr($html,0,$pos_inicial);
			$novoHtml .= '<table class="rodape" cellspacing="0" cellpadding="2" border="0" align="center" style="width: 100%;">
						<tbody><tr>
							<td height="2" colspan="2"></td>
						</tr>
						<tr>
							<td height="15" align="left">
								Data: '.date("d/m/Y").' - '.date("H:i:s").' /
									Último acesso ('.formata_data( $_SESSION['usuacesso']).') -
									<a href="javascript:abrirUsuariosOnline();">
										<span id="rdpUsuariosOnLine"></span>
										Usuários On-Line
									</a>
							</td>
														<td style="text-align: left;"><a href="javascript: abrirRegras( '.$this->mnuid.' );">Visualizar Regras</a></td>
														<td height="15" align="right">
								
									SIMEC - <a href="javascript:janela(\'/geral/fale_conosco.php?uc='.$_SESSION['usucpf'].'\',550,600)">Fale Conosco</a>
									<script type="text/javascript">
				function alertMe( objTag )
				{
					alert( objTag.title );
					return false;
				}
			</script>
			'.montaLinkManual( $_SESSION['sisid'] ).'									|
									Tx.: Cache de Sistema								
							</td>
						</tr>
					</tbody></table>';
			
			$novoHtml.= substr($html,$pos_final,strlen($html));
			echo $novoHtml;exit;
		}
		
	}
	
	public function apagarCache()
	{
		$arrSessao = $this->tratarSessao();
		if(is_array($arrSessao)){ // Loop no $_SESSION tratado para gerar a hash do cache
			foreach($arrSessao as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_POST)){ // Loop no $_POST para gerar a hash do cache
			foreach($_POST as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		if(is_array($_GET)){ // Loop no $_GET para gerar a hash do cache
			foreach($_GET as $keyCache => $arr){
				$string .= $keyCache.$arr;
			}
		}
		
		if($this->usucpf){ // Cache individual por usuário
			$string.=$this->usucpf;
		}
		
		if(!$this->diretorio){ //Verifica se não existe diretorio para o cache
			$this->diretorio = APPRAIZ."arquivos/".$_SESSION['sisdiretorio'];
		}
		
		$cacheFile = md5($string); //Nome do arquivo que armazenará o conteúdo.
		
		if(is_file($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html")){
			chmod($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html", 0666);
			if(unlink($this->diretorio."/cache/".$this->mnuid."/$cacheFile.html")){
				$this->arrInfo[] = "Arquivo de cache excluído com sucesso.";
			}else{
				$this->arrInfo[] = "Não foi possível excluir o arquivo de cache.";
			}
		}else{
			$this->arrInfo[] = "Arquivo de cache inexistente.";
		}
	}
	
	private function tratarSessao()
	{
		$arrSessao = $_SESSION; //Loop no $_SESSION para gerar a hash do cache
		unset($arrSessao['desenvolvedores']);
		unset($arrSessao['ambiente']);
		unset($arrSessao['theme_temp']);
		unset($arrSessao['sql']);
		unset($arrSessao['MSG_AVISO']);
		unset($arrSessao['NUM_IP_CLIENTE']);
		unset($arrSessao['DES_BROWSER']);
		unset($arrSessao['sisid']);
		unset($arrSessao['usucpf']);
		unset($arrSessao['usucpforigem']);
		unset($arrSessao['suscod']);
		unset($arrSessao['usutentativas']);
		unset($arrSessao['usunome']);
		unset($arrSessao['usuemail']);
		unset($arrSessao['sisdiretorio']);
		unset($arrSessao['sisarquivo']);
		unset($arrSessao['sisdsc']);
		unset($arrSessao['sisurl']);
		unset($arrSessao['sisabrev']);
		unset($arrSessao['sisexercicio']);
		unset($arrSessao['paginainicial']);
		unset($arrSessao['usunivel']);
		unset($arrSessao['susdataultacesso']);
		unset($arrSessao['superuser']);
		unset($arrSessao['usuacesso']);
		unset($arrSessao['evHoraUltimoAcesso']);
		unset($arrSessao['favurl']);
		unset($arrSessao['mnudsc']);
		unset($arrSessao['mnuid']);
		unset($arrSessao['theme_simec']);
		unset($arrSessao['menu']);
		unset($arrSessao['exercicioaberto']);
		unset($arrSessao['mostramsg']);
		return $arrSessao;
	}
	
	private function verificaCacheParametro()
	{
		$parametros['SESSION'] = $this->tratarSessao();
		$parametros['POST'] = $_POST;
		$parametros['GET'] = $_GET;
		$caprequisicao = simec_json_encode($parametros);
		
		$dataAtual = strtotime(date("YmdHis"));
		$sql = "select capdata,captipoexpira,capexpira,caprefazercache,capnaofazercache from seguranca.cacheparametro where caprequisicao = '$caprequisicao' and cacid = ".$this->cacid." and capstatus = 'A'";
		$arrDados = $this->pegaLinha($sql);
		if($arrDados['capnaofazercache'] == "f"){
			$this->naoFazerCache = true;
			$this->apagarCache();
			return false;
		}
		if($arrDados['caprefazercache'] == "t"){
			$this->apagarCache();
			$sql = "update seguranca.cacheparametro set caprefazercache = false where caprequisicao = '$caprequisicao' and cacid = ".$this->cacid;
			$this->executar($sql);
			$this->commit();
			return false;
		}
		$dataUltimoCache = strtotime($arrDados['capdata'],date("YmdHis"));
		switch($arrDados['captipoexpira']){
			case 'D':
				$Q = 86400;
				break; //dia
			case 'H':
				$Q = 3600;
				break; //hora
			case 'M':
				$Q = 60;
				break; //minuto
			case 'S':
				$Q = 1;
				break; //segundo
			default:
				$Q = false;
				break;
		}
		
		if(!$Q){
			return true;
		}else{
			$tempo = round( ($dataAtual - $dataUltimoCache) / $Q );
			if($tempo > $arrDados['capexpira']){
				$this->apagarCache();
				return false;
			}else{
				return true;
			}
		}
	}
	
}