<?php

/**
 * Modelo de Processos
 * @author Silas Matheus
 */
class Processo extends Modelo{
	
		
	/**
	 * Recupera registro para edicao
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do processo
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$rs = null;
		
		$id = str_replace('.', '', $id);
		$id = str_replace('-', '', $id);
		
		$id = addslashes($id);
		
		$server = "mecsrv14";
		$user   = "syssimec_sap";
		$pwd   = "sy12jn27db";
		$db     = "DBPSIDOC";

		try{
			
			$link = mssql_connect($server, $user, $pwd) or die("Couldn't connect to SQL Server");
			mssql_select_db($db, $link);
			$sql = "select Numero,Data from VW_consulta_processo WHERE numero='{$id}' ";
			$rs = mssql_fetch_row(mssql_query($sql));
			
		}catch(Exception $e){
			
			
		}
		
		return $rs;
		
	}
	
}

